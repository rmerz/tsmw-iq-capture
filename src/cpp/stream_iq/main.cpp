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

#include "../include/TSMWIQInterfaceFunc.h"
#include "../include/TSMWIQInterfaceTypes.h"
// Include filter specification for 1MHz sampling rate
#include "../include/Filter_1MHz.h"

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
                          block_length (1000000)
                          
  {}

  void parseCmd (int, char **);

  bool fileOutputFlag;
  char* pFilename;
  char* pDescription;

  (unsigned __int64) f1;
  (unsigned __int64) f2;
  unsigned int splitter;

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
    if (valid == false) {
      std::cerr << "Invalid option: " << argv[count] << std::endl;
      exit (-1);
    }
  }

}

class Util
{
public:
  void printLastError (int, std::ofstream*);
  void loadK1Interface (std::ofstream*);
  void releaseK1Interface (std::ofstream*);

  double get_iq_power (short scaling, double, double);
  double get_average_iq_power (short, double*, double*,
                               unsigned int);
};

void
Util::printLastError (int ErrorCode, std::ofstream* OutLog)
{
  char *pErrorText;

  // Use TSMWGetLastError_c to get error message and error code
  pErrorText = TSMWGetLastError_c ( &ErrorCode );
  std::cout << "TSMWIQStopStreaming_c: ErrorCode: " << ErrorCode << " ErrorText: " << pErrorText << std::endl;
  *OutLog   << "TSMWIQStopStreaming_c: ErrorCode: " << ErrorCode << " ErrorText: " << pErrorText << std::endl;
}

void
Util::loadK1Interface (std::ofstream* OutLog)
{
  int ErrorCode;

  ErrorCode = TSMWInitInterface_c ();
  if (ErrorCode == 0) {
    std::cout << "Initialized\n";
    *OutLog   << "Initialized\n";
  } else {
    printLastError (ErrorCode,OutLog);
  }
}

void
Util::releaseK1Interface (std::ofstream* OutLog)
{
  int ErrorCode;

  // Release interface (which closes connection to TSMW)
  ErrorCode = TSMWReleaseInterface_c ();
  if ( ErrorCode == 0){
    std::cout << "Released\n";
    *OutLog   << "Released\n";
  } else {
    printLastError (ErrorCode,OutLog);
  }
}

double
Util::get_iq_power (short scaling, double real, double imag)
{
  double scaling_lin_mV;

  // See TSMWIQPlotData.m: 2000 is for 100 * 20 (voltage)
  scaling_lin_mV = std::pow(10,(double)scaling/100/20);
  // Factor of 2 is because we assume a CW (sine/cosine) is sent.
  return 10*std::log10((std::pow(real*scaling_lin_mV,2) + std::pow(imag*scaling_lin_mV,2))/2);
}

double
Util::get_average_iq_power (short scaling, double* real, double* imag,
                            unsigned int NoOfBlockSamples)
{
  double scaling_lin_mV;
  double power_sum = 0;

  scaling_lin_mV = std::pow(10,(double)scaling/100/20);
  for (unsigned int CntSample = 0; CntSample < NoOfBlockSamples; CntSample++ ) {
    power_sum = power_sum +
      std::pow(real[CntSample]*scaling_lin_mV,2) +
      std::pow(imag[CntSample]*scaling_lin_mV,2);
  }
  return 10*std::log10(power_sum/(2*NoOfBlockSamples));
}



int
main (int argc, char *argv[], char *envp[])
{
  // Logfile
  std::ofstream* OutLog = new std::ofstream("log.txt", std::ios::out);

  int ErrorCode;
  CaptureOptions options;
  Util util;

  options.parseCmd (argc,argv);

  char IPAddress[] = "192.168.0.2";
  TSMW_IQIF_MODE_t TSMWMode;
  TSMWMode.Frontends = 3;               // enable both frontends
  TSMWMode.AMPS_CH1 = (1ULL << 32) - 1; // all preselector bands
  TSMWMode.AMPS_CH2 = (1ULL << 32) - 1; // all preselector bands
  TSMWMode.Mode = 0;                    // standard mode  (has to be zero)
  unsigned short TSMWID;

  std::cout << "Preselectors: " << TSMWMode.AMPS_CH1 << " " << TSMWMode.AMPS_CH2 << "\n";

  TSMW_IQIF_MEAS_CTRL_t MeasCtrl;
  MeasCtrl.NoOfSamples = 1; // Number of IQ samples to measure
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
  unsigned int NoOfBlockSamples = options.block_length;

  // Find out how many (sub-) channels are measured
  unsigned int NoOfChannels;
  if (pChannelCtrl1 == NULL) {
    // Frontend 2 used only
    NoOfChannels = pChannelCtrl2->NoOfChannels;
    std::cout << "FE2 freq: " << ChannelCtrl2.Frequency << std::endl;
  } else if (pChannelCtrl2 == NULL) {
    // Frontend 1 used only
    NoOfChannels = pChannelCtrl1->NoOfChannels;
    std::cout << "FE1 freq: " << ChannelCtrl1.Frequency << std::endl;
  } else {
    // Both frontends used
    NoOfChannels = pChannelCtrl1->NoOfChannels + pChannelCtrl2->NoOfChannels;
    std::cout << "FE1 freq: " << ChannelCtrl1.Frequency << std::endl;
    std::cout << "FE2 freq: " << ChannelCtrl2.Frequency << std::endl;
  }
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
  pReal = (double*) malloc (NoOfChannels * NoOfBlockSamples * sizeof(double));
  double* pImag;
  pImag = (double*) malloc (NoOfChannels * NoOfBlockSamples * sizeof(double));

  std::cout << "Number of samples per block: " << NoOfBlockSamples << "\n";

  // Initialize TSMW IQ Interface
  util.loadK1Interface (OutLog);

  // Connect to TSMW
  ErrorCode = TSMWConnect_c (IPAddress, &TSMWMode, &TSMWID);
  if (ErrorCode == 0) {

    std::cout << "Connected\n";
    *OutLog   << "Connected\n";

    // Wait two seconds after connection establishment for frontend synchronization.
    std::cout << "Wait 5 seconds for frontend synchronization\n";
    *OutLog   << "Wait 5 seconds for frontend synchronization\n";
    clock_t trigger, seconds = 2;
    trigger = seconds * CLOCKS_PER_SEC + clock();
    while (trigger > clock());

    // Send user-specific resampling filter to TSMW
    ErrorCode = TSMWIQSetup_c (TSMWID, &Filter_1MHzParam, Filter_1MHzCoeff);
    if (ErrorCode == 0) {
      std::cout << "Filter set\n";
      *OutLog   << "Filter set\n";
      // Start streaming with predefined measurement and streaming
      // parameters Passing a NULL vector for pChannelCtrl1 or
      // pChannelCtrl2 means that frontend 1 or frontend 2,
      // respectively shall NOT be used for streamimg.
      ErrorCode = TSMWIQStream_c (TSMWID, &MeasCtrl, pChannelCtrl1, pChannelCtrl2, &StreamCtrl,
                                  options.pFilename, options.pDescription, (unsigned int)1);
      if (ErrorCode == 0){
        std::cout << "Streaming started\n";
        *OutLog   << "Streaming started\n";
        std::cout << "Press any key to interrupt\n";

        if (options.pFilename == NULL) {
          // Continuously get and process streaming data until key pressed
          unsigned int CntBlock = 0;
          double iq_power = 0;
          double iq_average_power = 0;
          unsigned int channel_offset = 0;
          do {
            // Get streaming data, wait for a stream data block up to
            // TimeOut seconds This function will always deliver the
            // next NoOfBlockSamples I/Q samples (for
            // online-processing)
            ErrorCode = TSMWIQGetStreamDouble_c ((unsigned char)StreamCtrl.StreamID, TimeOut, &IQResult,
                                                 pReal, pImag, pScaling, pOverFlow,
                                                 pCalibrated, Offset,
                                                 NoOfBlockSamples, NoOfChannels);
            if (ErrorCode == 0) {
              std::cout << "Block " << CntBlock << " received\n";

              // Display samples for each sub-channel

              for (unsigned int CntChannel = 0; CntChannel < NoOfChannels; CntChannel++) {
                // Sample offset between two successive channels
                channel_offset = CntChannel*NoOfBlockSamples;
                std::cout << "Debug: " << channel_offset << std::endl;
                std::cout << "Channel: " << CntChannel+1 << " / " << NoOfChannels
                          << " (first sample scaling,real,imag): "
                          << pScaling[CntChannel] << " " << pReal[channel_offset] << " " << pImag[channel_offset]
                          << std::endl;

                iq_power = util.get_iq_power (pScaling[CntChannel],
                                              pReal[channel_offset],
                                              pImag[channel_offset]);
                std::cout << "Channel: " << CntChannel+1 << " / " << NoOfChannels
                          << " (first sample IQ power): "
                          << iq_power << " dBm" << std::endl;

                if (pOverFlow[CntChannel] > 0)
                  std::cout << "Channel: " << CntChannel+1 << " / " << NoOfChannels << ": " << pOverFlow[CntChannel] << std::endl;

                // Average power over all samples
                iq_average_power = util.get_average_iq_power (pScaling[CntChannel],
                                                              &pReal[channel_offset],
                                                              &pImag[channel_offset],
                                                              NoOfBlockSamples);
                std::cout << "Channel: " << CntChannel+1 << " / " << NoOfChannels
                          << " (avg. IQ power): "
                          << iq_average_power << " dBm" << std::endl;
              }
            } else {
              util.printLastError (ErrorCode,OutLog);
            }
            CntBlock = CntBlock + 1;
            if (_kbhit()) {
              *OutLog   << "Number of blocks: " << CntBlock << std::endl;
            }
          } while (!_kbhit());
          // delete OutBlock;
        } else {
          // Loop as long as no keyboard key is hit
          while (!_kbhit());
        }

        // Stop streaming
        ErrorCode = TSMWIQStopStreaming_c ( TSMWID, (unsigned char)StreamCtrl.StreamID, &StreamStatus);
        if ( ErrorCode == 0 ){
          std::cout << "Streaming stopped\n";
          *OutLog   << "Streaming stopped\n";
        } else {
          util.printLastError (ErrorCode,OutLog);
        }
      } else {
        util.printLastError (ErrorCode,OutLog);
      }
    } else {
      util.printLastError (ErrorCode,OutLog);
    }
  } else {
    util.printLastError (ErrorCode,OutLog);
  }

  util.releaseK1Interface (OutLog);

  // while (_kbhit()) {
  //   char ch = _getch();
  // }
  // std::cout << "Press any key to quit program\n";
  // while(!_kbhit());

  delete OutLog;
  return (0);
}
