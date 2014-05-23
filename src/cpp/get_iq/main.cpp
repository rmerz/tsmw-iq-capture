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
                          f1 (1000000000),
                          f2 (0),
                          splitter (0),
                          block_length (1000000),
			  max_number_of_blocks (UINT_MAX)
                          
  {}

  void parseCmd (int, char **);

  bool fileOutputFlag;
  char* pFilename;
  char* pDescription;
  bool verbose;

  (unsigned __int64) f1;
  (unsigned __int64) f2;
  unsigned int splitter;

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

  for (int count = 1; count < argc; count++) {
    // std::cout << "  argv[" << count << "]   "
    //        << argv[count] << "\n";
    valid = false;
    if (help.compare (argv[count]) == 0 || short_help.compare (argv[count]) == 0) {
      std::cout << "-f\t\tWrite IQ stream to file\n"
		<< "-v\t\tVerbose mode: print IQ samples summary\n"
                << "--filename [string]\tSpecify file name to write IQ samples (default is iq_data.dat)\n"
                << "--description [string]\tSpecify description to attach with IQ samples capture (default is n/a)\n"
                << "--fe1_freq [double]\tFrequency of frontend 1 in Hz.\n\t\t\tIf frequency is 0, deactivates frontend 1 (default is 1e9)\n"
                << "--fe2_freq [double]\tFrequency of frontend 2 in Hz.\n\t\t\tIf frequency is 0, deactivates frontend 2 (default is 0 e.g inactive)\n"
                << "--splitter [1|0]\tActivate splitter from FE1 to FE2 (default is inactive)\n"
                << "--block_length [INT]\tSize in bits of the measurement blocks (default is 1e6)\n"
                << "--n [UINT]\tMax. number of blocks to capture (default is unlimited)\n"
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

int
main (int argc, char *argv[], char *envp[])
{
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

  printf ("Preselectors: %lu %lu\n", TSMWMode.AMPS_CH1, TSMWMode.AMPS_CH2);

  TSMW_IQIF_MEAS_CTRL_t MeasCtrl;
  MeasCtrl.NoOfSamples = options.block_length; // Number of IQ samples to measure
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
  // second channel etc e.g. streaming on frontend 1 (FE1) with 2
  // channels (CH1, CH2) and on frontend 2 with 3 channels gives a
  // vector as follows: [ (FE1 CH1) (FE1 CH2) (FE2 CH1) (FE2 CH2) (FE2
  // CH3) ] length: NoOfChannels * NoOfBlockSamples
  double* pReal;
  pReal = (double*) malloc (NoOfChannels * MeasCtrl.NoOfSamples * sizeof(double));
  double* pImag;
  pImag = (double*) malloc (NoOfChannels * MeasCtrl.NoOfSamples * sizeof(double));

  printf ("Number of samples per block: %u\n", MeasCtrl.NoOfSamples);

  char header[512];
  FILE * binary_trace;
  if (options.fileOutputFlag) {
    binary_trace = fopen (options.pFilename, "wb");
    // Header for binary format
    sprintf_s (header, 512, "Header is %d bytes long. FE1 freq uint64 %u [1], FE2 freq uint64 %u [1], NoOfChannels uint %u [1], blockSize uint %u [1]. For each block: block IQtimeStart uint64 %u [1], Fsample double %u [1], short scaling %u [NoOfChannels], double real %u [blockSize*NoOfChannels], double imag %u [blockSize*NoOfChannels]",
	       sizeof (header),
	       sizeof (ChannelCtrl1.Frequency),
	       sizeof (ChannelCtrl2.Frequency),
	       sizeof (NoOfChannels),
	       sizeof (MeasCtrl.NoOfSamples),
	       sizeof (IQResult.StartTimeIQ),
	       sizeof (IQResult.Fsample),
	       sizeof (*pScaling),
	       sizeof (*pReal), sizeof (*pImag));
    printf ("Header for binary trace: %s\n", header);
    fwrite (&header, sizeof (header), 1, binary_trace);
    fwrite (&ChannelCtrl1.Frequency, sizeof (ChannelCtrl1.Frequency), 1, binary_trace);
    fwrite (&ChannelCtrl2.Frequency, sizeof (ChannelCtrl2.Frequency), 1, binary_trace);
    fwrite (&NoOfChannels, sizeof (NoOfChannels), 1, binary_trace);
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
    //ErrorCode = TSMWIQSetup_c (TSMWID, &Filter_1MHzParam, Filter_1MHzCoeff);
    ErrorCode = TSMWIQSetup_c (TSMWID, &Filter_110kHzParam, Filter_110kHzCoeff);
    if (ErrorCode == 0) {
      printf ("Filter set");

      printf ("Entering measurement loop\n");
      printf ("Press any key to interrupt\n");

      unsigned long MeasRequestID;

      // Continuously get and process streaming data until key pressed
      unsigned int CntBlock = 0;
      double iq_power = 0;
      double iq_average_power = 0;
      unsigned int channel_offset = 0;
      do {
	CntBlock = CntBlock + 1;
	// Schedule measurement
	ErrorCode = TSMWIQMeasure_c (TSMWID, &MeasRequestID, NULL, 0,
				     &MeasCtrl, pChannelCtrl1, pChannelCtrl2);
	// Get data for MeasRequestID, wait for a data block up to
	// TimeOut seconds
	ErrorCode = TSMWIQGetDataDouble_c (TSMWID, MeasRequestID, TimeOut, &IQResult,
					   pReal, pImag, pScaling,
					   pOverFlow, pCalibrated,
					   MeasCtrl.NoOfSamples, NoOfChannels, 0, 0);
	if (ErrorCode == 0) {
	  // printf ("Block %d received: %u\n",CntBlock,IQResult.NoOfSamples);
	  printf ("Block %d received\n",CntBlock);

	  if (options.verbose) {
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
	      printf ("Channel %u/%u: (first sample IQ power) %.2f dBm\n",
		      CntChannel+1,NoOfChannels,
		      iq_power);

	      if (pOverFlow[CntChannel] > 0)
		printf ("Channel %u/%u: (overflow) %lu\n",pOverFlow[CntChannel]);

	      // Average power over all samples
	      iq_average_power = util.get_average_iq_power (pScaling[CntChannel],
							    &pReal[channel_offset],
							    &pImag[channel_offset],
							    MeasCtrl.NoOfSamples);
	      printf ("Channel %u/%u: (avg. IQ power) %.2f dBm\n",
		      CntChannel+1,NoOfChannels,
		      iq_average_power);
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
