/**
 * TSMW Project   TSMW Streaming Demo with online processing
 *
 * @file         main.cpp
 * @abstract     This demo application demonstrates seamless streaming. It starts a I/Q stream measurement
 *               and processes the I/Q data online in order to find gaps in the I/Q stream (which should never occur).
 *               In order to do that it is assumed that a CW signal of approximately -40dBm at 1.000001GHz is fed
 *               into the TSMW. When measuring at 1GHz, the resulting I and Q components will show a sinusiodal
 *               variation but no abrupt changes. This allows for detecting gaps in the I/Q stream be looking
 *               for abrupt changes of the I or Q component.
 *
 *               This demo application now procecces the streamed I/Q data in order to find such gaps and produces
 *               an error output if in a processed I/Q block a gap is detected. If no input signal is present, only
 *               noise is measured, which produces one error output per processed data block.
 *
 * @copyright     (c) 2008 Rohde & Schwarz GmbH & Co. KG, Munich
 * @author        Markus Herdin, Johannes Dommel, Hubert Willmann
 * @version
 *    20.10.2008
 *
 */

#include "TSMWIQInterfaceFunc.h"
#include "TSMWIQInterfaceTypes.h"

// Include filter specification for 1MHz sampling rate
#include "Filter_1MHz.h"

#include "stdafx.h"

void
printLastError (int ErrorCode, std::ofstream* OutLog)
{
  char *pErrorText;

  // Use TSMWGetLastError_c to get error message and error code
  pErrorText = TSMWGetLastError_c ( &ErrorCode );
  std::cout << "TSMWIQStopStreaming_c: ErrorCode: " << ErrorCode << " ErrorText: " << pErrorText << std::endl;
  *OutLog   << "TSMWIQStopStreaming_c: ErrorCode: " << ErrorCode << " ErrorText: " << pErrorText << std::endl;
}

void
loadK1Interface (std::ofstream* OutLog)
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
releaseK1Interface (std::ofstream* OutLog)
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

int main()
{
  std::ofstream* OutLog = new std::ofstream("log.txt", std::ios::out); // logfile
  bool FileOutputFlag = true; // flag to indicate whether first
                              // errorblock shall be written into file
                              // or not
  int ErrorCode;

  char IPAddress[] = "192.168.0.2";
  TSMW_IQIF_MODE_t TSMWMode;
  TSMWMode.Frontends = 3;               // enable both frontends
  TSMWMode.AMPS_CH1 = (1ULL << 32) - 1; // all preselector bands
  TSMWMode.AMPS_CH2 = (1ULL << 32) - 1; // all preselector bands
  TSMWMode.Mode = 0;                    // standard mode  (has to be zero)
  unsigned short TSMWID;

  std::cout << "Preselectors: " << TSMWMode.AMPS_CH1 << " " << TSMWMode.AMPS_CH2 << "\n";

  char *pFileName = NULL; // if NULL then online streaming mode
  char *pDescription = NULL;
  //char *pFileName = "data.dat"; // if NULL then online streaming mode
  //char *pDescription = "Test file";
  unsigned int CreateIfExists = 1; // Only necessary for streaming to file

  TSMW_IQIF_MEAS_CTRL_t MeasCtrl;
  MeasCtrl.NoOfSamples = 1; // Number of IQ samples to measure
  MeasCtrl.FilterType = 1;  // Use userdefined filters (0 corresponds to pre-defined filters)
  MeasCtrl.FilterID = 1;    // Number of the filter that shall be used
  MeasCtrl.DataFormat = 3;  // IQ-data compression format, 3: 20 Bit / 2 is 12 Bit
  MeasCtrl.AttStrategy = 0; // Attenuation strategy, currently unused, shall be set to zero
  MeasCtrl.Splitter = 0;    // 0: Disable splitter (splits the signal
			    // from frontend 1 after the preselector
			    // to both frontends)
  MeasCtrl.Priority = 15;   // Relative priority, Valid range: 0 .. 15, 15 highest


  TSMW_IQIF_CH_CTRL_t ChannelCtrl1;
  TSMW_IQIF_CH_CTRL_t *pChannelCtrl1 = &ChannelCtrl1;
  ChannelCtrl1.Frequency = (unsigned __int64)1.8385e9; // Center frequency in Hz
  ChannelCtrl1.UseOtherFrontend = 0; // Reserved for future use, has to be zero
  ChannelCtrl1.NoOfChannels = 1;     // Number of channels that shall be used (1..4)
  ChannelCtrl1.Attenuation = 0;      // Attenuation to use (0..15dB)
  ChannelCtrl1.Preamp = 1;           // 1: Enable
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
  ChannelCtrl2.Frequency = (unsigned __int64)1.0e9;
  ChannelCtrl2.UseOtherFrontend = 0;
  ChannelCtrl2.NoOfChannels = 1;
  ChannelCtrl2.Attenuation = 0;
  ChannelCtrl2.Preamp = 1;
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

  pChannelCtrl2 = NULL;                           // uncomment this line for use of frontend 1 only

  unsigned int TimeOut = 10000; // in ms

  unsigned __int64 Offset = 0;
  unsigned int NoOfBlockSamples = (unsigned int)1e6; // Block size for processing: a too large value will cause a segfault

  // Find out how many (sub-) channels are measured
  unsigned int NoOfChannels;
  if (pChannelCtrl1 == NULL) {
    // Frontend 2 used only
    NoOfChannels = pChannelCtrl2->NoOfChannels;
  } else if (pChannelCtrl2 == NULL) {
    // Frontend 1 used only
    NoOfChannels = pChannelCtrl1->NoOfChannels;
  } else {
    // Both frontends used
    NoOfChannels = pChannelCtrl1->NoOfChannels + pChannelCtrl2->NoOfChannels;
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
  loadK1Interface (OutLog);

  // Connect to TSMW
  ErrorCode = TSMWConnect_c (IPAddress, &TSMWMode, &TSMWID);
  if (ErrorCode == 0) {

    std::cout << "Connected\n";
    *OutLog   << "Connected\n";

    // Wait two seconds after connection establishment for frontend synchronization.
    std::cout << "Wait 5 seconds for frontend synchronization\n";
    *OutLog   << "Wait 5 seconds for frontend synchronization\n";
    //Sleep (5000);
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
                                  pFileName, pDescription, CreateIfExists);
      if (ErrorCode == 0){
        std::cout << "Streaming started\n";
        *OutLog   << "Streaming started\n";
        std::cout << "Press any key to interrupt\n";

	if (pFileName == NULL) {
	  // Open output log file
	  //std::ofstream* OutBlock = new std::ofstream("iq_blocks.dat", std::ios::out);
	  // Continuously get and process streaming data until key pressed
	  unsigned int CntBlock = 0;
	  double iq_power = 0;
	  double scaling_lin_mV = 0;
	  double real_scaled = 0;
	  double imag_scaled = 0;
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

	      // Display (or copy to file) samples for each sub-channel

	      for (unsigned int CntChannel = 0; CntChannel < NoOfChannels; CntChannel++) {
		// Sample offset between two successive channels
		channel_offset = CntChannel*NoOfBlockSamples;
		std::cout << channel_offset << std::endl;
		std::cout << "Channel: " << CntChannel+1 << " / " << NoOfChannels
			  << " (first sample scaling,real,imag): "
			  << pScaling[CntChannel] << " " << pReal[channel_offset] << " " << pImag[channel_offset]
			  << std::endl;

		// See TSMWIQPlotData.m: 2000 is for 100 * 20
		scaling_lin_mV = pow(10,pScaling[CntChannel]/100/20);
		real_scaled = pReal[channel_offset]*scaling_lin_mV;
		imag_scaled = pImag[channel_offset]*scaling_lin_mV;
		// std::cout << "Channel: " << CntChannel << " / " << NoOfChannels << ": " << scaling_lin_mV << " " << real_scaled << " " << imag_scaled << std::endl;
		iq_power = 10*log10((pow(real_scaled,2) + pow(imag_scaled,2))/2);
		std::cout << "Channel: " << CntChannel+1 << " / " << NoOfChannels
			  << " (first sample IQ power): "
			  << iq_power << " dBm" << std::endl;

		if (pOverFlow[CntChannel] > 0)
		  std::cout << "Channel: " << CntChannel+1 << " / " << NoOfChannels << ": " << pOverFlow[CntChannel] << std::endl;

		// for (unsigned int CntSample = 1; CntSample < NoOfBlockSamples; CntSample++ ) {
		// 	// pReal[ CntSample + d ];
		// 	// pImag[ CntSample + d ];
		// 	for (unsigned int CntSample = 0; CntSample < NoOfBlockSamples; CntSample++ ) {
		// 	  //*OutBlock << CntChannel << " " << std::setw(15) << std::setprecision(15) << pReal[ CntSample + d ] << std::endl;
		// 	  std::cout << CntChannel << " " << std::setw(15) << std::setprecision(15) << pReal[ CntSample + d ] << std::endl;
		// 	}
		// }
	      }
	    } else {
	      printLastError (ErrorCode,OutLog);
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
          printLastError (ErrorCode,OutLog);
        }
      } else {
        printLastError (ErrorCode,OutLog);
      }
    } else {
      printLastError (ErrorCode,OutLog);
    }
  } else {
    printLastError (ErrorCode,OutLog);
  }

  releaseK1Interface (OutLog);

  // while (_kbhit()) {
  //   char ch = _getch();
  // }
  // std::cout << "Press any key to quit program\n";
  // while(!_kbhit());

  delete OutLog;
  return (0);
}
