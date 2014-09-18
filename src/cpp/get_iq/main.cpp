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

#include "../include/zmq.h" // Needs to be before windows.h
#include <windows.h>
#include <conio.h>
#include <assert.h>
#include <iomanip>
#include <fstream>
#include <cmath>

#include <iostream>
#include <tchar.h>

#include "../common/util.h"
// Include filter specification for 2 MS/s and 0.22 MS/s sampling rate
#include "../include/Filter_5MHz.h"
#include "../include/Filter_1MHz.h"
#include "../include/Filter_110kHz.h"

class CaptureOptions
{
private:

public:
  CaptureOptions (void) : fileOutputFlag (false),
                          pFilename (NULL),
                          pDescription (NULL),
                          verbose (false),
                          trigger (false),
                          zmq (false),
                          f1 (1000000000),
                          f2 (0),
                          splitter (0),
                          block_length (1000000),
			  filter_id (1),
                          max_number_of_blocks (UINT_MAX)
                          
  {}

  void parseCmd (int, char **);

  bool fileOutputFlag;
  char* pFilename;
  char* pDescription;
  bool verbose;
  bool trigger;
  bool zmq;

  (unsigned __int64) f1;
  (unsigned __int64) f2;
  unsigned int splitter;
  unsigned int filter_id;

  (unsigned int) max_number_of_blocks;
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
  std::string verbose_opt ("-v");
  std::string block_opt ("-n");
  std::string trigger_opt ("-t");
  std::string filter_opt ("--filter_id");
  std::string zmq_opt ("-z");


  for (int count = 1; count < argc; count++) {
    // std::cout << "  argv[" << count << "]   "
    //        << argv[count] << "\n";
    valid = false;
    if (help.compare (argv[count]) == 0 || short_help.compare (argv[count]) == 0) {
      std::cout << "-f\t\tWrite IQ measurement blocks to file\n"
                << "-v\t\tVerbose mode: print IQ samples summary\n"
                << "--filename [string]\tSpecify file name to write IQ samples (default is iq_data.dat)\n"
                << "--description [string]\tSpecify description to attach with IQ samples capture (default is n/a)\n"
                << "--fe1_freq [double]\tFrequency of frontend 1 in Hz.\n\t\t\tIf frequency is 0, deactivates frontend 1 (default is 1e9)\n"
                << "--fe2_freq [double]\tFrequency of frontend 2 in Hz.\n\t\t\tIf frequency is 0, deactivates frontend 2 (default is 0 e.g inactive)\n"
                << "--splitter [1|0]\tActivate splitter from FE1 to FE2 (default is inactive)\n"
                << "--block_length [INT]\tSize in bits of the measurement blocks (default is 1e6)\n"
                << "--filter_id [INT]\tFilter identifier (default is 1, available 0, 5, 110)\n"
                << "-n [UINT]\tMax. number of blocks to capture (default is unlimited)\n"
                << "-t\t\tActive external trigger mode (default is inactive)\n"
                << "-z\t\tActivate zeroMQ publishing (default is inactive)\n"
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
    if (block_opt.compare (argv[count]) == 0) {
      assert (argc >= count+1);
      count++;
      max_number_of_blocks = (unsigned int)atoi (argv[count]);
      valid = true;
    }
    if (verbose_opt.compare (argv[count]) == 0) {
      verbose = true;
      valid = true;
    }
    if (zmq_opt.compare (argv[count]) == 0) {
      zmq = true;
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

int
main (int argc, char *argv[], char *envp[])
{
  int ErrorCode;
  CaptureOptions options;
  Util util;
  // zmq specific
  void *context;
  void *publisher;

  options.parseCmd (argc,argv);

  int zmq_rc = 0;
  if (options.zmq) {
    // Prepare our context and publisher
    context = zmq_ctx_new ();
    publisher = zmq_socket (context, ZMQ_PUB);
    int hwm = 1;
    zmq_rc = zmq_setsockopt (publisher, ZMQ_SNDHWM, &hwm, sizeof(hwm));
    assert (zmq_rc == 0);
    size_t hwm_size = sizeof (hwm);
    zmq_rc = zmq_getsockopt (publisher, ZMQ_SNDHWM,  &hwm, &hwm_size);
    assert (zmq_rc == 0);
    printf ("High water mark set to %d\n",hwm);
    zmq_rc = zmq_bind (publisher, "tcp://127.0.0.1:5556");
    assert (zmq_rc == 0);
    // unsigned int nb_channel = 2;
    // double scale = 2.3;
    // double test[4] = {3.1415,1.4,2.0,-9};

    // unsigned int msg_len = sizeof (nb_channel) + sizeof (scale) + sizeof (test);
    // char *msg_pack = new char[msg_len]();

    // memcpy (&msg_pack[0], &nb_channel, sizeof (nb_channel));
    // memcpy (&msg_pack[sizeof (nb_channel)], &scale, sizeof (scale));
    // memcpy (&msg_pack[sizeof (nb_channel)+sizeof (scale)], &test, sizeof (test));

    // unsigned int count = 0;
    // while (1) {
    //   rc = zmq_send (publisher, msg_pack, msg_len, ZMQ_DONTWAIT);
    //   printf ("Push %u\n",count);
    //   printf ("%d\n",msg_len);
    //   printf ("%d, %d, %d, %d\n",msg_len,sizeof (char),sizeof(int),sizeof(double));
    //   count += 1;
    // }
  }


  char IPAddress[] = "192.168.0.2";
  TSMW_IQIF_MODE_t TSMWMode;
  TSMWMode.Frontends = 3;               // enable both frontends
  TSMWMode.AMPS_CH1 = (1ULL << 32) - 1; // all preselector bands
  TSMWMode.AMPS_CH2 = (1ULL << 32) - 1; // all preselector bands
  TSMWMode.Mode = 0;                    // standard mode  (has to be zero)
  unsigned short TSMWID;

  printf ("Preselectors: %lu %lu\n", TSMWMode.AMPS_CH1, TSMWMode.AMPS_CH2);

  TSMW_IQIF_MEAS_CTRL_t MeasCtrl;
  MeasCtrl.NoOfSamples = options.block_length; // Number of IQ samples to measure
  if (options.filter_id == 0)
    MeasCtrl.FilterType = 0;  // Use userdefined filters (0 corresponds to pre-defined filters)
  else
    MeasCtrl.FilterType = 1;
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
 
  if (options.f1 == 0) {
      // Disable frontend 1 only
    pChannelCtrl1 = NULL;
  }
  if (options.f2 == 0) {
      // Disable frontend 2 only
    pChannelCtrl2 = NULL;
  }

  if (options.trigger && pChannelCtrl1 == NULL) {
    printf ("Disabling frontend 1 with triggered measurements is not supported.\n");
    exit (-1);
  }

  unsigned int TimeOut = 60000; // in ms

  unsigned __int64 Offset = 0;
 // Block size for processing: a too large value will cause a segfault
  // unsigned int NoOfBlockSamples = options.block_length;

  // Find out how many (sub-) channels are measured
  unsigned int NoOfChannels = util.getNumberOfChannels (pChannelCtrl1,
                                                        pChannelCtrl2);
  printf ("Total number of channels: %u\n", NoOfChannels);

  TSMW_IQIF_RESULT_t IQResult;
  // Create array of variables receiving scaling, overflow and
  // calibrated information from interface when requesting I/Q data.
  short *pScaling = (short*) malloc(NoOfChannels * sizeof(short));
  unsigned long *pOverFlow = (unsigned long*) malloc(NoOfChannels * sizeof(unsigned long));
  unsigned int *pCalibrated = (unsigned int*) malloc(NoOfChannels * sizeof(unsigned int));

  // The output samples of different frontends / sub-channels are
  // delivered in a single vector.  The order is data of first
  // sub-channel of first frontend, data of second sub-channel of
  // second channel etc e.g. on frontend 1 (FE1) with 2
  // channels (CH1, CH2) and on frontend 2 with 3 channels gives a
  // vector as follows: [ (FE1 CH1) (FE1 CH2) (FE2 CH1) (FE2 CH2) (FE2
  // CH3) ] length: NoOfChannels * NoOfBlockSamples
  double* pReal;
  pReal = (double*) malloc (NoOfChannels * MeasCtrl.NoOfSamples * sizeof(double));
  double* pImag;
  pImag = (double*) malloc (NoOfChannels * MeasCtrl.NoOfSamples * sizeof(double));

  printf ("Number of samples per block: %u\n", MeasCtrl.NoOfSamples);

  // For zmq
  unsigned int msg_pack_len = sizeof (NoOfChannels) + sizeof (MeasCtrl.NoOfSamples)
    + (sizeof (IQResult.Fsample))
    + (sizeof (IQResult.StartTimeIQ))
    + (sizeof (pScaling)+sizeof (pOverFlow))*NoOfChannels
    + (sizeof (double))*(2*NoOfChannels * MeasCtrl.NoOfSamples);
  char* msg_pack = new char[msg_pack_len]();
  unsigned int msg_pack_offset = 0;
  memcpy (&msg_pack[msg_pack_offset], &NoOfChannels, sizeof (NoOfChannels));
  msg_pack_offset += sizeof (NoOfChannels);
  memcpy (&msg_pack[msg_pack_offset], &MeasCtrl.NoOfSamples, sizeof (MeasCtrl.NoOfSamples));
  msg_pack_offset += sizeof (MeasCtrl.NoOfSamples);

  TSMW_IQIF_TRIG_CTRL_t TriggerParam;
  TSMW_IQIF_TRIG_CTRL_t *pTriggerParam = &TriggerParam;
  if (options.trigger) {
    printf ("Trigger mode configuration\n");
    TriggerParam.Cmd = 0;  // 0: Start triggering (when used in TSMWIQMeasureTrig command)
                           // 1: Stop triggered measurement
                           // 2: Change attenuator and preamp setting
    TriggerParam.Mode = 0; // Trigger mode, has to be zero
    TriggerParam.Falling = 0; // Trigger edge, 0: rising, 1: falling
    TriggerParam.TriggerLine = 1; // 1: Use trigger input 1
                                  // 2: Use trigger input 2
                                  // 3: Trigger on both inputs
    TriggerParam.MeasRequestID;  // Meas.request ID of
                                 // period. meas.req., only used
                                 // when parameters of a triggered
                                 // measurement
    TriggerParam.Att[2]; // New attenuator setting for each channel (only used when Cmd == 2)
    TriggerParam.Preamp[2]; // New preamp setting for each channel
                            // (only used when Cmd == 2)
  }

  char header[512];
  FILE * binary_trace;
  if (options.fileOutputFlag) {
    binary_trace = fopen (options.pFilename, "wb");
    // Header for binary format
    sprintf_s (header, 512, "Header is %d bytes long. FE1_freq uint64 %u [1], FE2_freq uint64 %u [1], NoOfChannelsFE1 uint %u [1], NoOfChannelsFE2 uint %u [1], blockSize uint %u [1]. For each block: blockIQtimeStart uint64 %u [1], Fsample double %u [1], scaling  short %u [(NoOfChannelsFE1+NoOfChannelsFE2)], real double %u [blockSize*(NoOfChannelsFE1+NoOfChannelsFE2)], imag double %u [blockSize*(NoOfChannelsFE1+NoOfChannelsFE2)]",
               sizeof (header),
               sizeof (ChannelCtrl1.Frequency),
               sizeof (ChannelCtrl2.Frequency),
	       sizeof (ChannelCtrl1.NoOfChannels),
               sizeof (ChannelCtrl2.NoOfChannels),
               sizeof (MeasCtrl.NoOfSamples),
               sizeof (IQResult.StartTimeIQ),
               sizeof (IQResult.Fsample),
               sizeof (*pScaling),
               sizeof (*pReal), sizeof (*pImag));
    printf ("Header for binary trace: %s\n", header);
    fwrite (&header, sizeof (header), 1, binary_trace);
    fwrite (&ChannelCtrl1.Frequency, sizeof (ChannelCtrl1.Frequency), 1, binary_trace);
    fwrite (&ChannelCtrl2.Frequency, sizeof (ChannelCtrl2.Frequency), 1, binary_trace);
    fwrite (&ChannelCtrl1.NoOfChannels, sizeof (unsigned int), 1, binary_trace);
    fwrite (&ChannelCtrl2.NoOfChannels, sizeof (unsigned int), 1, binary_trace);
    fwrite (&MeasCtrl.NoOfSamples, sizeof (MeasCtrl.NoOfSamples), 1, binary_trace);
  }

  // Initialize TSMW IQ Interface
  util.loadK1Interface ();

  // Connect to TSMW
  ErrorCode = TSMWConnect_c (IPAddress, &TSMWMode, &TSMWID);
  if (ErrorCode == 0) {

    printf ("Connected\n");

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
	  if (options.filter_id != 0) {
	    printf ("Invalid filter id.\n");
	    exit (-1);
	  }

    if (ErrorCode == 0) {
      printf ("Filter set\n");

      printf ("Entering measurement loop\n");
      printf ("Press any key to interrupt\n");

      unsigned long MeasRequestID[2];

      // Continuously get and process data until key pressed
      unsigned int CntBlock = 0;
      double iq_power = 0;
      double angle = 0;
      // double iq_average_power = 0;
      double* result;
      result = (double*) malloc (2 * sizeof (double));
      unsigned int channel_offset = 0;
      unsigned int msg_offset = 0;
      do {
        CntBlock = CntBlock + 1;
        // Schedule measurement
        if (options.trigger) {
	  ErrorCode = TSMWIQMeasureTrig_c (TSMWID, &MeasRequestID[0], NULL, 0,
					   &MeasCtrl, pChannelCtrl1, NULL,
					   pTriggerParam);
	  if (pChannelCtrl2 != NULL)
	    ErrorCode = TSMWIQMeasureTrig_c (TSMWID, &MeasRequestID[1], NULL, 0,
					     &MeasCtrl, NULL, pChannelCtrl2,
					     pTriggerParam);
        } else {
          ErrorCode = TSMWIQMeasure_c (TSMWID, &MeasRequestID[0], NULL, 0,
                                       &MeasCtrl, pChannelCtrl1, pChannelCtrl2);
        }
        // Get data for MeasRequestID, wait for a data block up to
        // TimeOut seconds
        if (options.trigger) {
	  ErrorCode = TSMWIQGetDataDouble_c (TSMWID, MeasRequestID[0], TimeOut, &IQResult,
					     pReal, pImag, pScaling,
					     pOverFlow, pCalibrated,
					     MeasCtrl.NoOfSamples, 1, 0, 0);
	  printf ("Check: %lu\n",IQResult.StartTimeIQ);
	  if (pChannelCtrl2 != NULL) {
	    ErrorCode = TSMWIQGetDataDouble_c (TSMWID, MeasRequestID[1], TimeOut, &IQResult,
					       &pReal[MeasCtrl.NoOfSamples*ChannelCtrl1.NoOfChannels], &pImag[MeasCtrl.NoOfSamples*ChannelCtrl1.NoOfChannels], &pScaling[1],
					       &pOverFlow[1], &pCalibrated[1],
					       MeasCtrl.NoOfSamples, 1, 0, 0);
	    printf ("Check: %lu\n",IQResult.StartTimeIQ);
	  }

	} else {
	  ErrorCode = TSMWIQGetDataDouble_c (TSMWID, MeasRequestID[0], TimeOut, &IQResult,
					     pReal, pImag, pScaling,
					     pOverFlow, pCalibrated,
					     MeasCtrl.NoOfSamples, NoOfChannels, 0, 0);
	}


        if (ErrorCode == 0) {
          // printf ("Block %d received: %u\n",CntBlock,IQResult.NoOfSamples);
          // printf ("Block %d received\n",CntBlock);

	  if (options.zmq) {
	    msg_offset = msg_pack_offset;
	    // printf ("%d\n",msg_offset);

	    memcpy (&msg_pack[msg_offset], &IQResult.Fsample, sizeof (IQResult.Fsample));
	    msg_offset += sizeof (IQResult.Fsample);

	    // printf ("%d\n",msg_offset);
	    memcpy (&msg_pack[msg_offset],
		    &IQResult.StartTimeIQ, sizeof (IQResult.StartTimeIQ));
	    msg_offset += sizeof (IQResult.StartTimeIQ);

	    // printf ("%d %d %d\n",msg_offset,sizeof (*pScaling),sizeof (*pScaling)*NoOfChannels);
	    memcpy (&msg_pack[msg_offset],
		    pScaling, sizeof (*pScaling)*NoOfChannels);
	    msg_offset += sizeof (*pScaling)*NoOfChannels;

	    // printf ("%d\n",msg_offset);
	    memcpy (&msg_pack[msg_offset],
		    pOverFlow, sizeof (*pOverFlow)*NoOfChannels);
	    msg_offset += sizeof (*pOverFlow)*NoOfChannels;

	    // printf ("%d\n",msg_offset);
	    memcpy (&msg_pack[msg_offset],
		    pReal, sizeof (*pReal)*NoOfChannels*MeasCtrl.NoOfSamples);
	    msg_offset += sizeof (*pReal)*NoOfChannels*MeasCtrl.NoOfSamples;

	    // printf ("%d\n",msg_offset);
	    memcpy (&msg_pack[msg_offset],
		    pImag, sizeof (*pImag)*NoOfChannels*MeasCtrl.NoOfSamples);
	    msg_offset += sizeof (*pImag)*NoOfChannels*MeasCtrl.NoOfSamples;

	    zmq_rc = zmq_send (publisher, msg_pack, msg_pack_len, ZMQ_DONTWAIT);
	    assert (zmq_rc);
	  }

          if (options.verbose) {
            printf ("Block %d received\n",CntBlock);
            // Display samples for each sub-channel
            for (unsigned int CntChannel = 0; CntChannel < NoOfChannels; CntChannel++) {
              // Sample offset between two successive channels
              channel_offset = CntChannel*MeasCtrl.NoOfSamples;
              printf ("Channel %u/%u: (first sample scaling,re,im) %d, %.2f, %.2f\n",
                       CntChannel+1,NoOfChannels,
                       pScaling[CntChannel], pReal[channel_offset], pImag[channel_offset]);

              iq_power = util.get_iq_power (pScaling[CntChannel],
                                            pReal[channel_offset],
                                            pImag[channel_offset]);
              angle = util.get_angle (pReal[channel_offset],pImag[channel_offset]);
              printf ("Channel %u/%u: (first sample IQ power/angle) %.2f dBm, %.2f deg.\n",
                      CntChannel+1,NoOfChannels,
                      iq_power,angle);

              if (pOverFlow[CntChannel] > 0)
                printf ("Channel %u/%u: (OVERFLOW) %lu\n",pOverFlow[CntChannel]);

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
              printf ("Channel %u/%u: (avg. IQ power/angle) %.2f dBm, %.2f deg.\n",
                      CntChannel+1,NoOfChannels,
                      result[0],result[1]);
            }
          }
          if (options.fileOutputFlag) {
            fwrite (&IQResult.StartTimeIQ, sizeof (IQResult.StartTimeIQ), 1, binary_trace);
            fwrite (&IQResult.Fsample, sizeof (IQResult.Fsample), 1, binary_trace);
            fwrite (pScaling, sizeof (*pScaling), NoOfChannels, binary_trace);
            fwrite (pReal, sizeof (*pReal), NoOfChannels*MeasCtrl.NoOfSamples, binary_trace);
            fwrite (pImag, sizeof (*pImag), NoOfChannels*MeasCtrl.NoOfSamples, binary_trace);
          }

        } else {
          util.printLastError (ErrorCode);
        }
        if (CntBlock >= options.max_number_of_blocks)
          break;
      } while (!_kbhit());
      printf ("Number of blocks: %u\n",CntBlock);
      if (options.fileOutputFlag) {
        fclose (binary_trace);
      }
    } else {
      util.printLastError (ErrorCode);
    }
  } else {
    util.printLastError (ErrorCode);
  }

  util.releaseK1Interface ();

  return (0);
}
