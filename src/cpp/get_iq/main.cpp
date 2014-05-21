/**
 * TSMW Project  TSMW IQ samples capture tool: get just one block
 *
 * @file         main.cpp
 * @abstract     TBD
 *
 * @copyright    (c) 2014 Swisscom AG, Bern, Schweiz
 * @author       Ruben Merz
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
                          f1 (1000000000),
                          f2 (0),
                          splitter (0)                          
  {}

  void parseCmd (int, char **);

  bool fileOutputFlag;
  char* pFilename;
  char* pDescription;

  (unsigned __int64) f1;
  (unsigned __int64) f2;
  unsigned int splitter;
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

  std::cout << "Preselectors: " << TSMWMode.AMPS_CH1 << " " << TSMWMode.AMPS_CH2 << "\n";

  TSMW_IQIF_MEAS_CTRL_t MeasCtrl;
  TSMW_IQIF_MEAS_CTRL_t *pMeasCtrl = &MeasCtrl;
  MeasCtrl.NoOfSamples = 48000; // Number of IQ samples to measure
  MeasCtrl.FilterType = 0;  // Use userdefined filters (0 corresponds to pre-defined filters)
  MeasCtrl.FilterID = 1;    // Number of the filter that shall be used
  MeasCtrl.DataFormat = 3;  // IQ-data compression format, 3: 20 Bit / 2 is 12 Bit
  MeasCtrl.AttStrategy = 0; // Attenuation strategy, currently unused, shall be set to zero
  MeasCtrl.Splitter = options.splitter; // 0: Disable splitter (splits
                                        // the signal from frontend 1
                                        // after the preselector to
                                        // both frontends)
  MeasCtrl.Priority = 15;   // Relative priority, Valid range: 0 .. 15, 15 highest

  std::cout << "Number of samples requested: " << MeasCtrl.NoOfSamples << std::endl;

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

  if (options.f1 == 0) {
      // Disable frontend 1
    pChannelCtrl1 = NULL;
  }
  if (options.f2 == 0) {
      // Disable frontend 2
    pChannelCtrl2 = NULL;
  }
  // Find out how many (sub-) channels are measured
  unsigned int NoOfChannels = util.getNumberOfChannels (pChannelCtrl1,pChannelCtrl2);
  std::cout << "Number of channels: " << NoOfChannels << std::endl;

  // Initialize TSMW IQ Interface
  util.loadK1Interface ();

  // Connect to TSMW
  unsigned int timeOut = 10000; // in ms
  ErrorCode = TSMWConnect_c (IPAddress, &TSMWMode, &TSMWID);
  if (ErrorCode != 0) {
    util.printLastError (ErrorCode);
    exit (-1);
  }
  std::cout << "Connected to " << TSMWID << std::endl;
  util.waitForFrontendSync ();

  // ErrorCode = TSMWIQSetup_c (TSMWID, &Filter_1MHzParam, Filter_1MHzCoeff);
  // if (ErrorCode != 0) {
  //   util.kill (ErrorCode);
  // }
  // std::cout << "Filter set" << std::endl;

  unsigned long MeasRequestID;
  unsigned __int64 *pStartTimes = (unsigned __int64*) malloc (sizeof (unsigned __int64));
  // ErrorCode = TSMWIQMeasure_c (TSMWID, &MeasRequestID, pStartTimes, 0,
  // 			       pMeasCtrl,pChannelCtrl1,pChannelCtrl2);
  TSMW_IQIF_TRIG_CTRL_t TriggerParam;
  TSMW_IQIF_TRIG_CTRL_t *pTriggerParam = &TriggerParam;
  TriggerParam.Cmd = 0;                   // 0: Start triggering (when used in TSMWIQMeasureTrig command)
                                           // 1: Stop triggered measurement
                                           // 2: Change attenuator and preamp setting
  TriggerParam.Mode = 0;                  // Trigger mode, has to be zero
  TriggerParam.Falling = 0;               // Trigger edge, 0: rising, 1: falling
  TriggerParam.TriggerLine = 3;           // 1: Use trigger input 1
                                           // 2: Use trigger input 2
                                           // 3: Trigger on both inputs
  //TriggerParam.MeasRequestID;         // Meas.request ID of period. meas.req., only used when changing
                                           // parameters of a triggered measurement
  //TriggerParam.Att[2];                // New attenuator setting for each channel (only used when Cmd == 2)
  //TriggerParam..Preamp[2];             // New preamp setting for each channel (only used when Cmd == 2)

  ErrorCode = TSMWIQMeasureTrig_c (TSMWID, &MeasRequestID, pStartTimes, 0,
				   pMeasCtrl,pChannelCtrl1,pChannelCtrl2,
				   pTriggerParam);
  if (ErrorCode != 0) {
    util.kill (ErrorCode);
  }
  std::cout << "Measurement " << MeasRequestID << " configured" << std::endl;

  long NoOfIQResults;
  TSMW_IQIF_RESULT_t IQResult;
  do {
    // ErrorCode = TSMWIQGetResultParam_c (0,timeOut,&IQResult);
    // if (ErrorCode != 0) {
    //   util.kill (ErrorCode);
    // }
    // std::cout << "Result parameters, request " << IQResult.MeasRequestID << ": " << IQResult.NoOfSamples << std::endl;

    // Returns the number of measurement results available (and not the number of samples)
    ErrorCode = TSMWIQDataAvailable_c (&NoOfIQResults);
    if (ErrorCode != 0) {
      util.kill (ErrorCode);
    }
    std::cout << "Number of results available: " << NoOfIQResults << std::endl;

  } while (NoOfIQResults == 0);

  unsigned int NoOfSamples = MeasCtrl.NoOfSamples;
  double* pReal = (double*) malloc (NoOfChannels * NoOfSamples * sizeof(double));
  double* pImag = (double*) malloc (NoOfChannels * NoOfSamples * sizeof(double));
  short* pScaling = (short*) malloc(NoOfChannels * sizeof(short));
  unsigned long* pOvfl = (unsigned long*) malloc(NoOfChannels * sizeof(unsigned long));
  unsigned int *pCalibrated = (unsigned int*) malloc(NoOfChannels * sizeof(unsigned int));

  ErrorCode = TSMWIQGetDataDouble_c (TSMWID, MeasRequestID, timeOut, &IQResult,
				     pReal,
				     pImag,
				     pScaling,
				     pOvfl,
				     pCalibrated,
				     1, NoOfChannels,
				     0, 0);
  if (ErrorCode != 0) {
    util.kill (ErrorCode);
  }

  ErrorCode = TSMWIQGetResultParam_c (0,timeOut,&IQResult);
  if (ErrorCode != 0) {
    util.kill (ErrorCode);
  }
  std::cout << IQResult.MeasRequestID << " "
	    << IQResult.StartTimeIQ << " "
	    << IQResult.StartTimeHost << " "
	    << IQResult.Fsample << " "
	    << IQResult.NoOfSamples << std::endl;

  util.releaseK1Interface ();
  return (0);
}
