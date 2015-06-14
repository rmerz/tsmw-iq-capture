/**
 * TSMW Project  TSMW IQ sample capture tool
 *
 * @file         main.cpp
 * @abstract     TBD
 *
 * @copyright    (c) 2014 Swisscom AG, Bern, Schweiz
 *               (c) 2008 Rohde & Schwarz GmbH & Co. KG, Munich
 * @author       Ruben Merz
 *               Markus Herdin, Johannes Dommel, Hubert Willmann
 * @version
 *    May 2014
 *
 */
#include <Afxwin.h>
#include "../common/util.h"
#include "../common/serial.h"
// Include filter specification for 2 MS/s and 0.22 MS/s sampling rate
#include "../include/Filter_1MHz.h"
#include "../include/Filter_5MHz.h"
#include "../include/Filter_110kHz.h"
#include "../include/Filter_500kHz.h"
#include "../include/Filter_500kHz_b.h"

#include "stdafx.h"

class CaptureOptions
{
private:

public:
  CaptureOptions (void) : fileOutputFlag (false),
                          pFilename (NULL),
                          pDescription (NULL),
                          f1 (1000000000),
                          f2 (0),
                          splitter (0),
                          trigger (false),
                          port_number (5),
                          block_length (1000000),
			  filter_id (1)
                          
  {}

  void parseCmd (int, char **);

  bool fileOutputFlag;
  char* pFilename;
  char* pDescription;

  (unsigned __int64) f1;
  (unsigned __int64) f2;
  unsigned int splitter;
  bool trigger;
  unsigned int port_number;
  unsigned int filter_id;

  (unsigned int) block_length;
};

void
CaptureOptions::parseCmd (int argc, char *argv[])
{
  // Display each command-line argument
  if (argc > 1)
    std::cout << "Parse command-line" << std::endl;
  else
    return;

  bool valid = false;
  std::string to_file ("-f");
  std::string iq_filename ("--filename");
  std::string iq_filename_description ("--description");
  std::string fe_splitter ("--splitter");
  std::string fe1_freq ("--fe1_freq");
  std::string fe2_freq ("--fe2_freq");
  std::string block_length_opt ("--block_length");
  std::string filter_opt ("--filter_id");
  std::string trigger_opt ("--trigger");
  std::string port_number_opt ("--port_number");
  std::string help ("--help");
  std::string short_help ("-h");

  for (int count = 1; count < argc; count++) {
    // std::cout << "  argv[" << count << "]   "
    //        << argv[count] << "\n";
    valid = false;
    if (help.compare (argv[count]) == 0 || short_help.compare (argv[count]) == 0) {
      std::cout << "-f\t\tWrite IQ stream to file\n"
                << "--filename [string]\tSpecify file name to write IQ samples (default is iq_data.dat)\n"
                << "--description [string]\tSpecify description to attach with IQ samples capture (default is n/a)\n"
                << "--fe1_freq [double]\tFrequency of frontend 1 in Hz.\n\t\t\tIf frequency is 0, deactivates frontend 1 (default is 1e9)\n"
                << "--fe2_freq [double]\tFrequency of frontend 2 in Hz.\n\t\t\tIf frequency is 0, deactivates frontend 2 (default is 0 e.g inactive)\n"
                << "--splitter\tActivate splitter from FE1 to FE2 (default is inactive)\n"
                << "--block_length\tSize in bits of the measurement blocks (default is 1e6)\n"
                << "--filter_id [INT]\tFilter identifier (default is 1, available 0, 5, 110)\n"
                << "--trigger Waits on the serial-line box to start (and stop) the measurement"
                << "--port-number Port number to use for serial-line communication (default is 5)"
                << "--help|-h\tPrints this help message\n" << std::endl;
      exit (0);
    }
    if (to_file.compare (argv[count]) == 0) {
      fileOutputFlag = true;
      pFilename = (char*) malloc (256*sizeof(char));
      sprintf_s (pFilename, 256, "iq_data.dat");
      pDescription = (char*) malloc (1024*sizeof(char));
      sprintf_s (pDescription, 1024, "n/a");
      valid = true;
    }
    // fileOutputFlag needs to be activated first
    if (fileOutputFlag == true && iq_filename.compare (argv[count]) == 0) {
      assert (argc >= count+1);
      count++;
      sprintf_s (pFilename, 256, argv[count]);
      valid = true;
    }
    if (fileOutputFlag == true && iq_filename_description.compare (argv[count]) == 0) {
      assert (argc >= count+1);
      count++;
      sprintf_s (pDescription, 1024, argv[count]);
      valid = true;
    }
    if (fe_splitter.compare (argv[count]) == 0) {
      splitter = 1;
      valid = true;
    }
    if (trigger_opt.compare (argv[count]) == 0) {
      trigger = true;
      valid = true;
    }
    if (port_number_opt.compare (argv[count]) == 0) {
      assert (argc >= count+1);
      count++;
      port_number = (unsigned int)atof (argv[count]);
      valid = true;
    }
    if (fe1_freq.compare (argv[count]) == 0) {
      assert (argc >= count+1);
      count++;
      f1 = (unsigned __int64)atof (argv[count]);
      valid = true;
    }
    if (fe2_freq.compare (argv[count]) == 0) {
      assert (argc >= count+1);
      count++;
      f2 = (unsigned __int64)atof (argv[count]);
      valid = true;
    }
    if (block_length_opt.compare (argv[count]) == 0) {
      assert (argc >= count+1);
      count++;
      block_length = (unsigned int)atof (argv[count]);
      valid = true;
    }
    if (filter_opt.compare (argv[count]) == 0) {
      assert (argc >= count+1);
      count++;
      filter_id = (unsigned int)atof (argv[count]);
      valid = true;
    }
    if (valid == false) {
      std::cerr << "Invalid option: " << argv[count] << std::endl;
      exit (-1);
    }
  }
}

typedef struct triggerParam {
  bool run;
  unsigned int port_number;
} triggerParam;

UINT triggerStatus (LPVOID pParam)
{
  CSerial serial;
  triggerParam* p = (triggerParam*)pParam;
  int j=0;

  printf ("Running state: %d\n",p->run);
  printf ("Serial port number: %u\n",p->port_number);

  if (p->port_number > 10)
    printf ("[WARNING] In case of failure when opening the port, try to set the port number <= 10");
  
  if (serial.Open (p->port_number,19200)) { // COM port hardcoded
    printf("Port %u opened successfully\n",p->port_number);

    assert (p->run == false);

    while (TRUE){
      char* ding = new char[1];
      int nBytesRead = serial.ReadData(ding, 1);
      if (nBytesRead > 0){
        std::string s=ding;
        char ss=s[0]; // Take first char of ding
        if ((ss=='{') || (ss=='}')){
          // Do something when mirror is detected
          printf ("%d - Mirror detected: state change\n",j);
          // Big fat assumptions: (1) this thread is the only one
          // _writing_ to p->run; (2) the write is atomic
          if (p->run == true)
            p->run = false;
          else
            p->run = true;
          j++;

          // Wait until we exit the mirror
          do {
            int nBytesRead = serial.ReadData(ding, 1);
                  
            if (nBytesRead > 0){
              std::string s=ding;
              ss=s[0];
            }
            else{
              ss='2';
            }
          } while ((ss == '2') || (ss=='{') || (ss=='}'));
        }
      }
      delete []ding;
    }
  }
  else
    printf ("[ERROR] Failed to open port!\n");

  return -1;   // thread completed successfully
}

int
main (int argc, char *argv[], char *envp[])
{
  int ErrorCode;
  CaptureOptions options;
  Util util;

  options.parseCmd (argc,argv);

  triggerParam p;
  p.run = false;  // Initial state
  p.port_number = options.port_number;
  if (options.trigger == true)
    AfxBeginThread (triggerStatus,&p);
  
  char IPAddress[] = "192.168.0.2";
  TSMW_IQIF_MODE_t TSMWMode;
  TSMWMode.Frontends = 3;               // enable both frontends
  TSMWMode.AMPS_CH1 = (1ULL << 32) - 1; // all preselector bands
  TSMWMode.AMPS_CH2 = (1ULL << 32) - 1; // all preselector bands
  TSMWMode.Mode = 0;                    // standard mode  (has to be zero)
  unsigned short TSMWID;

  std::cout << "Preselectors: " << TSMWMode.AMPS_CH1 << " " << TSMWMode.AMPS_CH2 << "\n";

  TSMW_IQIF_MEAS_CTRL_t MeasCtrl;
  MeasCtrl.NoOfSamples = options.block_length; // Number of IQ samples to measure
  if (options.filter_id == 0)
    MeasCtrl.FilterType = 0;
  else
    MeasCtrl.FilterType = 1;  // Use userdefined filters (0 corresponds to pre-defined filters)
  MeasCtrl.FilterID = 1;    // Number of the filter that shall be used
  MeasCtrl.DataFormat = 3;  // IQ-data compression format, 3: 20 Bit / 2 is 12 Bit
  MeasCtrl.AttStrategy = 0; // Attenuation strategy, currently unused, shall be set to zero
  MeasCtrl.Splitter = options.splitter; // 0: Disable splitter (splits
                                        // the signal from frontend 1
                                        // after the preselector to
                                        // both frontends)
  MeasCtrl.Priority = 15;   // Relative priority, Valid range: 0 .. 15, 15 highest


  TSMW_IQIF_CH_CTRL_t ChannelCtrl1;
  TSMW_IQIF_CH_CTRL_t *pChannelCtrl1 = &ChannelCtrl1;
  ChannelCtrl1.Frequency = options.f1; // Center frequency in Hz
  ChannelCtrl1.UseOtherFrontend = 0; // Reserved for future use, has to be zero
  if (ChannelCtrl1.Frequency == 0)
    ChannelCtrl1.NoOfChannels = 0;
  else
    ChannelCtrl1.NoOfChannels = 1;     // Number of channels that shall be used (1..4)
  ChannelCtrl1.Attenuation = 0;      // Attenuation to use (0..15dB)
  ChannelCtrl1.Preamp = 0;           // 1: Enable
  ChannelCtrl1.CalibInput = 0;       // 0: Disable
  ChannelCtrl1.FreqShift[0] = 0;     // Frequency shift from center frequency in Hz for each subchannel
  ChannelCtrl1.FreqShift[1] = 0;
  ChannelCtrl1.FreqShift[2] = 0;
  ChannelCtrl1.FreqShift[3] = 0;
  ChannelCtrl1.ChannelDelay[0] = 0; // Individual delay in taps for
                                    // each subchannel (after
                                    // filtering/resampling)
  ChannelCtrl1.ChannelDelay[1] = 0;
  ChannelCtrl1.ChannelDelay[2] = 0;
  ChannelCtrl1.ChannelDelay[3] = 0;
  ChannelCtrl1.BlockSize = 0;      // Reserved: must be set to 0
  ChannelCtrl1.BlockSkip = 0;      // Reserved
  ChannelCtrl1.DigIqOnOff = 0;

  TSMW_IQIF_CH_CTRL_t ChannelCtrl2;
  TSMW_IQIF_CH_CTRL_t *pChannelCtrl2 = &ChannelCtrl2;
  ChannelCtrl2.Frequency = options.f2;
  ChannelCtrl2.UseOtherFrontend = 0;
  if (ChannelCtrl2.Frequency == 0)
    ChannelCtrl2.NoOfChannels = 0;
  else
    ChannelCtrl2.NoOfChannels = 1;
  ChannelCtrl2.Attenuation = 0;
  ChannelCtrl2.Preamp = 0;
  ChannelCtrl2.CalibInput = 0;
  ChannelCtrl2.FreqShift[0] = 0;
  ChannelCtrl2.FreqShift[1] = 0;
  ChannelCtrl2.FreqShift[2] = 0;
  ChannelCtrl2.FreqShift[3] = 0;
  ChannelCtrl2.ChannelDelay[0] = 0;
  ChannelCtrl2.ChannelDelay[1] = 0;
  ChannelCtrl2.ChannelDelay[2] = 0;
  ChannelCtrl2.ChannelDelay[3] = 0;
  ChannelCtrl2.BlockSize = 0; // Reserved
  ChannelCtrl2.BlockSkip = 0; // Reserved
  ChannelCtrl2.DigIqOnOff = 0;

  TSMW_IQIF_STREAM_CTRL_t StreamCtrl;
  StreamCtrl.StreamID = 0;           // Stream ID, valid range: 0..15
  StreamCtrl.StreamBufferSize = 200; // Buffer size for streaming in
                                     // MBytes, a minimum of 200MB is
                                     // recommended
  StreamCtrl.MaxStreamSize = 4000;   // Maximum streaming size in MBytes.
 
  if (options.f1 == 0) {
      // Disable frontend 1 only
    pChannelCtrl1 = NULL;
  }
  if (options.f2 == 0) {
      // Disable frontend 2 only
    pChannelCtrl2 = NULL;
  }

  unsigned int TimeOut = 10000; // in ms

  unsigned __int64 Offset = 0;
 // Block size for processing: a too large value will cause a segfault
  // unsigned int NoOfBlockSamples = options.block_length;

  // Find out how many (sub-) channels are measured
  unsigned int NoOfChannels = util.getNumberOfChannels (pChannelCtrl1,
                                                        pChannelCtrl2);
  std::cout << "Total number of channels: " << NoOfChannels << "\n";

  TSMW_IQIF_STREAM_STATUS_t StreamStatus;
  TSMW_IQIF_RESULT_t IQResult;
  // Create array of variables receiving scaling, overflow and
  // calibrated information from interface when requesting I/Q data.
  short *pScaling = (short*) malloc(NoOfChannels * sizeof(short));
  unsigned long *pOverFlow = (unsigned long*) malloc(NoOfChannels * sizeof(unsigned long));
  unsigned int *pCalibrated = (unsigned int*) malloc(NoOfChannels * sizeof(unsigned int));

  // The output samples of different frontends / sub-channels are
  // delivered in a single vector.  The order is data of first
  // sub-channel of first frontend, data of second sub-channel of
  // second channel etc e.g. streaming on frontend 1 (FE1) with 2
  // channels (CH1, CH2) and on frontend 2 with 3 channels gives a
  // vector as follows: [ (FE1 CH1) (FE1 CH2) (FE2 CH1) (FE2 CH2) (FE2
  // CH3) ] length: NoOfChannels * NoOfBlockSamples
  double* pReal;
  pReal = (double*) malloc (NoOfChannels * MeasCtrl.NoOfSamples * sizeof(double));
  double* pImag;
  pImag = (double*) malloc (NoOfChannels * MeasCtrl.NoOfSamples * sizeof(double));

  std::cout << "Number of samples per block: " << MeasCtrl.NoOfSamples << "\n";

  // Initialize TSMW IQ Interface
  util.loadK1Interface ();

  // Connect to TSMW
  ErrorCode = TSMWConnect_c (IPAddress, &TSMWMode, &TSMWID);
  if (ErrorCode == 0) {

    std::cout << "Connected\n";

    util.waitForFrontendSync ();

    // Send user-specific resampling filter to TSMW
    if (options.filter_id == 1)
      ErrorCode = TSMWIQSetup_c (TSMWID, &Filter_1MHzParam, Filter_1MHzCoeff);
    else
      if (options.filter_id == 5)
	ErrorCode = TSMWIQSetup_c (TSMWID, &Filter_5MHzParam, Filter_5MHzCoeff);
      else
	if (options.filter_id == 110)
	  ErrorCode = TSMWIQSetup_c (TSMWID, &Filter_110kHzParam, Filter_110kHzCoeff);
	else
	  if (options.filter_id == 500)
	    ErrorCode = TSMWIQSetup_c (TSMWID, &Filter_500kHzParam, Filter_500kHzCoeff);
	  else
	    if (options.filter_id == 501)
	      ErrorCode = TSMWIQSetup_c (TSMWID, &Filter_500kHzParam, Filter_500kHz_bCoeff);
	    else
	      if (options.filter_id != 0) {
		printf ("Invalid filter id.\n");
		exit (-1);
	      }

    if (ErrorCode == 0) {
      std::cout << "Filter set\n";
      // Start streaming with predefined measurement and streaming
      // parameters Passing a NULL vector for pChannelCtrl1 or
      // pChannelCtrl2 means that frontend 1 or frontend 2,
      // respectively shall NOT be used for streamimg.
      ErrorCode = TSMWIQStream_c (TSMWID, &MeasCtrl, pChannelCtrl1, pChannelCtrl2, &StreamCtrl,
                                  options.pFilename, options.pDescription, (unsigned int)1);
      if (ErrorCode == 0){
        std::cout << "Streaming started\n";
        std::cout << "Press any key to interrupt\n";

        if (options.pFilename == NULL) {
          // Continuously get and process streaming data until key pressed
          unsigned int CntBlock = 0;
          double iq_power = 0;
	  double angle = 0;
          // double iq_average_power = 0;
	  double* result;
	  result = (double*) malloc (2 * sizeof(double));
          unsigned int channel_offset = 0;
          do {
            // Get streaming data, wait for a stream data block up to
            // TimeOut seconds This function will always deliver the
            // next NoOfBlockSamples I/Q samples (for
            // online-processing)
            ErrorCode = TSMWIQGetStreamDouble_c ((unsigned char)StreamCtrl.StreamID, TimeOut, &IQResult,
                                                 pReal, pImag, pScaling, pOverFlow,
                                                 pCalibrated, Offset,
                                                 MeasCtrl.NoOfSamples, NoOfChannels);
            if (ErrorCode == 0) {
              std::cout << "Block " << CntBlock << " received: " << IQResult.NoOfSamples << "\n";

              // Display samples for each sub-channel

              for (unsigned int CntChannel = 0; CntChannel < NoOfChannels; CntChannel++) {
                // Sample offset between two successive channels
                channel_offset = CntChannel*MeasCtrl.NoOfSamples;
                std::cout << "Debug: " << channel_offset << std::endl;
                std::cout << "Channel: " << CntChannel+1 << " / " << NoOfChannels
                          << " (first sample scaling,real,imag): "
                          << pScaling[CntChannel] << " " << pReal[channel_offset] << " " << pImag[channel_offset]
                          << std::endl;

                iq_power = util.get_iq_power (pScaling[CntChannel],
                                              pReal[channel_offset],
                                              pImag[channel_offset]);
		angle = util.get_angle (pReal[channel_offset],pImag[channel_offset]);
                std::cout << "Channel: " << CntChannel+1 << " / " << NoOfChannels
                          << " (first sample IQ power/angle): "
                          << iq_power << " dBm, " << angle << " deg." << std::endl;

                if (pOverFlow[CntChannel] > 0)
                  std::cout << "Channel: " << CntChannel+1 << " / " << NoOfChannels << " OVERFLOW: " << pOverFlow[CntChannel] << std::endl;

                // Average power over all samples
		util.get_average_iq_power_and_angle (result,
						     pScaling[CntChannel],
						     &pReal[channel_offset],
						     &pImag[channel_offset],
						     MeasCtrl.NoOfSamples);
                // iq_average_power = util.get_average_iq_power (pScaling[CntChannel],
                //                                               &pReal[channel_offset],
                //                                               &pImag[channel_offset],
                //                                               MeasCtrl.NoOfSamples);
                std::cout << "Channel: " << CntChannel+1 << " / " << NoOfChannels
                          << " (avg. IQ power/angle): "
                          << result[0] << " dBm, " << result[1] << " deg." << std::endl;
              }
            } else {
              util.printLastError (ErrorCode);
            }
            CntBlock = CntBlock + 1;
            if (_kbhit()) {
              std::cout   << "Number of blocks: " << CntBlock << std::endl;
            }
          } while (!_kbhit());
        } else {
          // Loop as long as no keyboard key is hit
          while (!_kbhit());
        }

        // Stop streaming
        ErrorCode = TSMWIQStopStreaming_c ( TSMWID, (unsigned char)StreamCtrl.StreamID, &StreamStatus);
        if ( ErrorCode == 0 ){
          std::cout << "Streaming stopped\n";
        } else {
          util.printLastError (ErrorCode);
        }
      } else {
        util.printLastError (ErrorCode);
      }
    } else {
      util.printLastError (ErrorCode);
    }
  } else {
    util.printLastError (ErrorCode);
  }

  util.releaseK1Interface ();

  // while (_kbhit()) {
  //   char ch = _getch();
  // }
  // std::cout << "Press any key to quit program\n";
  // while(!_kbhit());

  return (0);
}
