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


#include <conio.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <fstream>

int main()
{
  std::ofstream* OutLog = new std::ofstream("log.txt", std::ios::out); // logfile
  bool FileOutputFlag = true; // flag to indicate whether first
                              // errorblock shall be written into file
                              // or not

  int ErrorCode;
  char *pErrorText;

  char IPAddress[] = "192.168.0.2";
  TSMW_IQIF_MODE_t TSMWMode;
  TSMWMode.Frontends = 3;               // enable both frontends
  TSMWMode.AMPS_CH1 = (1ULL << 32) - 1; // all preselector bands
  TSMWMode.AMPS_CH2 = (1ULL << 32) - 1; // all preselector bands
  TSMWMode.Mode = 0;                    // standard mode  (has to be zero)
  unsigned short TSMWID;

  char *pFileName = NULL;          // if NULL then online streaming mode
  char *pDescription = NULL;
  unsigned int CreateIfExists = 1; // Only necessary for streaming to file

  TSMW_IQIF_MEAS_CTRL_t MeasCtrl;
  MeasCtrl.NoOfSamples = 1; // Number of IQ samples to measure
  MeasCtrl.FilterType = 1;  // Use userdefined filters
  MeasCtrl.FilterID = 1;    // Number of the filter that shall be used
  MeasCtrl.DataFormat = 3;  // IQ-data compression format, 3: 20 Bit
  MeasCtrl.AttStrategy = 0; // Attenuation strategy, currently unused, shall be set to zero
  MeasCtrl.Splitter = 1;    // 0: Disable splitter
  MeasCtrl.Priority = 15;   // Relative priority, Valid range: 0 .. 15, 15 highest


  TSMW_IQIF_CH_CTRL_t ChannelCtrl1;
  TSMW_IQIF_CH_CTRL_t *pChannelCtrl1 = &ChannelCtrl1;
  ChannelCtrl1.Frequency = (unsigned __int64)1.0e9; // Center frequency in Hz
  ChannelCtrl1.UseOtherFrontend = 0; // Reserved for future use, has to be zero
  ChannelCtrl1.NoOfChannels = 2;     // Number of channels that shall be used (1..4)
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


  TSMW_IQIF_CH_CTRL_t ChannelCtrl2;
  TSMW_IQIF_CH_CTRL_t *pChannelCtrl2 = &ChannelCtrl2;
  ChannelCtrl2.Frequency = (unsigned __int64)1.0e9;
  ChannelCtrl2.UseOtherFrontend = 0;
  ChannelCtrl2.NoOfChannels = 3;
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

  TSMW_IQIF_STREAM_CTRL_t StreamCtrl;
  StreamCtrl.StreamID = 0;           // Stream ID, valid range: 0..15
  StreamCtrl.StreamBufferSize = 200; // Buffer size for streaming in
                                     // MBytes, a minimum of 200MB is
                                     // recommended
  StreamCtrl.MaxStreamSize = 4000;   // Maximum streaming size in MBytes.

  // pChannelCtrl2 = NULL;                           // uncomment this line for use of frontend 1 only

  unsigned int TimeOut = 10000; // in ms

  unsigned __int64 Offset = 0;
  unsigned int NoOfBlockSamples = 100000; // Block size for processing

  double Threshold = 0.05; // Normalized difference between successive
                           // I/Q samples in order to detect an error
                           // (an abrupt change in I or Q component)

  // Find out how many (sub-) channels are measured
  unsigned int NoOfChannels;
  unsigned int ChFe1, ChFe2;
  if (pChannelCtrl1 == NULL) {
    // Frontend 2 used only
    ChFe2 = pChannelCtrl2->NoOfChannels;
    ChFe1 = 0;
    NoOfChannels = pChannelCtrl2->NoOfChannels;
  } else if (pChannelCtrl2 == NULL) {
    // Frontend 1 used only
    ChFe1 = pChannelCtrl1->NoOfChannels;
    ChFe2 = 0;
    NoOfChannels = pChannelCtrl1->NoOfChannels;
  } else {
    // Both frontends used
    ChFe1 = pChannelCtrl1->NoOfChannels;
    ChFe2 = pChannelCtrl2->NoOfChannels;
    NoOfChannels = pChannelCtrl1->NoOfChannels + pChannelCtrl2->NoOfChannels;
  }

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

  bool ErrorFlag = false; // Indicates whether error spurs occured or not

  // Initialize TSMW IQ Interface
  ErrorCode = TSMWInitInterface_c ();
  if (ErrorCode == 0) {
    std::cout << "Initialized\n";
    *OutLog   << "Initialized\n";

    // Connect to TSMW
    ErrorCode = TSMWConnect_c (IPAddress, &TSMWMode, &TSMWID);
    if (ErrorCode == 0) {

      std::cout << "Connected\n";
      *OutLog   << "Connected\n";

      // Wait two seconds after connection establishment for frontend synchronization.
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

          // Open output log file
          std::ofstream* OutBlock = new std::ofstream("ErrorBlock.dat", std::ios::out);

          // Continuously get and process streaming data until key pressed
          unsigned int CntBlock = 0;
          do {

            // Get streaming data, wait for a stream data block up to
            // TimeOut seconds This function will always deliver the
            // next NoOfBlockSamples I/Q samples (for
            // online-processing)
            ErrorCode = TSMWIQGetStreamDouble_c ((unsigned char)StreamCtrl.StreamID, TimeOut, &IQResult,
                                                  pReal, pImag, pScaling, pOverFlow, pCalibrated, Offset, NoOfBlockSamples, NoOfChannels);
            if (ErrorCode == 0) {
              std::cout << "Block " << CntBlock << " received\n";

              // Look for abrupt changes in I or Q component to find gaps
              // Do that for each sub-channel
              for (unsigned int CntChannel = 0; CntChannel < NoOfChannels; CntChannel++) {
                int d = CntChannel*NoOfBlockSamples; // Sample offset between two successive channels
                double MaxValue = pReal[ 0 + d ]; // Set to first sample of current block
                double MinValue = pReal[ 0 + d ]; // Set to first sample of current block
                double MaxDiff = 0;
                double RelDiff = 0;
                unsigned int MaxDiffIndex = 0;

                for (unsigned int CntSample = 1; CntSample < NoOfBlockSamples; CntSample++ ) {
                  if (pReal[ CntSample + d ] > MaxValue) // New maximum
                    MaxValue = pReal[ CntSample + d ];
                  if (pReal[ CntSample + d ] < MinValue) // New minimum
                    MinValue = pReal[ CntSample + d ];

                  double Diff = pReal[ CntSample + d ] - pReal[ CntSample + d - 1];
                  if (Diff > MaxDiff) { // New highest difference
                    MaxDiff = Diff;
                    MaxDiffIndex = CntSample - 1;
                  }
                }
                RelDiff = MaxDiff / (MaxValue - MinValue); // Scaling to 1 as highest difference
                if (RelDiff > Threashold) {
                  ErrorFlag = true; // Errors occured
                  unsigned int frontend, channel;
                  if (CntChannel+1 > ChFe1) {
                    // Channel belongs to frontend 2
                    frontend = 2;
                    channel = CntChannel+1 - ChFe1;
                  } else {
                    // Channel belongs to frontend 1
                    frontend = 1;
                    channel = CntChannel+1;
                  }
                  std::cout << "Error in Block: " << CntBlock << " Frontend: " << frontend << " Channel: " << channel << " Sample: " << MaxDiffIndex << " Diff: " << RelDiff << std::endl;

                  *OutLog  << "Error in Block: " << CntBlock << "     Frontend: " << frontend << "     Channel: " << channel << "     Sample: " << std::setw(7) << MaxDiffIndex << "     Diff: " << std::setw(8) << std::setprecision(6) << RelDiff << std::endl;
                  if (FileOutputFlag == true) {
                    *OutBlock << "Error in Block: " << CntBlock << "     Frontend: " << frontend << "     Channel: " << channel << "     Sample: " << MaxDiffIndex << "     Diff: " << RelDiff <<std::endl;
                    for (unsigned int CntSample = 0; CntSample < NoOfBlockSamples; CntSample++ ) {
                      *OutBlock << std::setw(15) << std::setprecision(15) << pReal[ CntSample + d ] << std::endl;
                    }
                    FileOutputFlag = false;
                  }
                }
              }
            } else {
              // Use TSMWGetLastError_c to get error message and error code
              pErrorText = TSMWGetLastError_c ( &ErrorCode );
              std::cout << "TSMWIQGetStreamDouble_c: ErrorCode: " << ErrorCode << " ErrorText: " << pErrorText << std::endl;
              *OutLog   << "TSMWIQGetStreamDouble_c: ErrorCode: " << ErrorCode << " ErrorText: " << pErrorText << std::endl;
            }
            CntBlock = CntBlock + 1;
            if (_kbhit()) {
              *OutLog   << "Number of blocks: " << CntBlock << std::endl;
            }
          } while (!_kbhit());


          if (ErrorFlag == false) {
            *OutLog  << "No errors\n";
            *OutBlock << "No errors\n";
          } else {
            *OutLog  << "Errors occured\n";
          }

          delete OutBlock;

          // Stop streaming
          ErrorCode = TSMWIQStopStreaming_c ( TSMWID, (unsigned char)StreamCtrl.StreamID, &StreamStatus);
          if ( ErrorCode == 0 ){
            std::cout << "Streaming stopped\n";
            *OutLog   << "Streaming stopped\n";


          } else {
            // Use TSMWGetLastError_c to get error message and error code
            pErrorText = TSMWGetLastError_c ( &ErrorCode );
            std::cout << "TSMWIQStopStreaming_c: ErrorCode: " << ErrorCode << " ErrorText: " << pErrorText << std::endl;
            *OutLog   << "TSMWIQStopStreaming_c: ErrorCode: " << ErrorCode << " ErrorText: " << pErrorText << std::endl;
          }
        } else {
          // Use TSMWGetLastError_c to get error message and error code
          pErrorText = TSMWGetLastError_c ( &ErrorCode );
          std::cout << "TSMWIQStream_c: ErrorCode: " << ErrorCode << " ErrorText: " << pErrorText << std::endl;
          *OutLog   << "TSMWIQStream_c: ErrorCode: " << ErrorCode << " ErrorText: " << pErrorText << std::endl;
        }
      } else {
        // Use TSMWGetLastError_c to get error message and error code
        pErrorText = TSMWGetLastError_c ( &ErrorCode );
        std::cout << "TSMWIQSetup_c: ErrorCode: " << ErrorCode << " ErrorText: " << pErrorText << std::endl;
        *OutLog   << "TSMWIQSetup_c: ErrorCode: " << ErrorCode << " ErrorText: " << pErrorText << std::endl;
      }
    } else {
      // Use TSMWGetLastError_c to get error message and error code
      pErrorText = TSMWGetLastError_c ( &ErrorCode );
      std::cout << "TSMWConnect_c: ErrorCode: " << ErrorCode << " ErrorText: " << pErrorText << std::endl;
      *OutLog   << "TSMWConnect_c: ErrorCode: " << ErrorCode << " ErrorText: " << pErrorText << std::endl;
    }

    // Release interface (which closes connection to TSMW)
    ErrorCode = TSMWReleaseInterface_c ();
    if ( ErrorCode == 0){
      std::cout << "Released\n";
      *OutLog   << "Released\n";

    } else {
      // Use TSMWGetLastError_c to get error message and error code
      pErrorText = TSMWGetLastError_c ( &ErrorCode );
      std::cout << "TSMWReleaseInterface_c: ErrorCode: " << ErrorCode << " ErrorText: " << pErrorText << std::endl;
      *OutLog   << "TSMWReleaseInterface_c: ErrorCode: " << ErrorCode << " ErrorText: " << pErrorText << std::endl;
    }
  } else {
    // Use TSMWGetLastError_c to get error message and error code
    pErrorText = TSMWGetLastError_c ( &ErrorCode );
    std::cout << "TSMWInitInterface_c: ErrorCode: " << ErrorCode << " ErrorText: " << pErrorText << std::endl;
    *OutLog   << "TSMWInitInterface_c: ErrorCode: " << ErrorCode << " ErrorText: " << pErrorText << std::endl;
  }

  while (_kbhit()) {
    char ch = _getch();
  }
  std::cout << "Press any key to quit program\n";
  while(!_kbhit());

  delete OutLog;
  return(0);
}
