/**
 * TSMW Project  Convert TSMW binary files to TBD format
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
#include <math.h>

#include <iomanip>
#include <fstream>
#include <iostream>
#include <string>

#include <tchar.h>

#include "../include/TSMWIQInterfaceFunc.h"
#include "../include/TSMWIQInterfaceTypes.h"

class ExtractOptions
{
private:

public:
  void parseCmd (int, char **);
  void printUsage (char* program);

  std::string iq_file_location;
};

void
ExtractOptions::printUsage (char* program)
{
  std::cout << program << " [-h|--help] filename\n" << std::endl;
}

void
ExtractOptions::parseCmd (int argc, char *argv[])
{
  // Display each command-line argument
  if (argc != 2) {
    printUsage (argv[0]);
    exit (0);
  }

  bool valid = false;
  std::string help ("--help");
  std::string short_help ("-h");

  for (int count = 1; count < argc; count++) {
    valid = false;
    if (help.compare (argv[count]) == 0 || short_help.compare (argv[count]) == 0) {
      printUsage (argv[0]);
      exit (0);
    }
    // Last option: must be the file location of the file to extract
    if (count+1 >= argc) {
      iq_file_location.assign(argv[count]);
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
  void printLastError (int);
  void loadK1Interface ();
  void releaseK1Interface ();

  double get_iq_power (short scaling, double, double);
  double get_average_iq_power (short, double*, double*,
                               unsigned int, unsigned int);
};

void
Util::printLastError (int ErrorCode)
{
  char *pErrorText;

  // Use TSMWGetLastError_c to get error message and error code
  pErrorText = TSMWGetLastError_c ( &ErrorCode );
  std::cout << "TSMWIQStopStreaming_c: ErrorCode: " << ErrorCode << " ErrorText: " << pErrorText << std::endl;
}

void
Util::loadK1Interface ()
{
  int ErrorCode;

  ErrorCode = TSMWInitInterface_c ();
  if (ErrorCode == 0) {
    std::cout << "Initialized\n";
  } else {
    printLastError (ErrorCode);
  }
}

void
Util::releaseK1Interface ()
{
  int ErrorCode;

  // Release interface (which closes connection to TSMW)
  ErrorCode = TSMWReleaseInterface_c ();
  if ( ErrorCode == 0){
    std::cout << "Released\n";
  } else {
    printLastError (ErrorCode);
  }
}

double
Util::get_iq_power (short scaling, double real, double imag)
{
  double scaling_lin_mV;

  // See TSMWIQPlotData.m: 2000 is for 100 * 20 (voltage)
  scaling_lin_mV = pow(10,(double)scaling/100/20);
  // Factor of 2 is because we assume a CW (sine/cosine) is sent.
  return 10*log10((pow(real*scaling_lin_mV,2) + pow(imag*scaling_lin_mV,2))/2);
}

double
Util::get_average_iq_power (short scaling, double* real, double* imag,
                            unsigned int offset, unsigned int blockSize)
{
  double scaling_lin_mV;
  double power_sum = 0;

  for (unsigned int k = 0; k < blockSize; k++ ) {
    power_sum = power_sum +
      pow(real[offset+k],2) +
      pow(imag[offset+k],2);
  }
  // return 10*log10(power_sum*pow(scaling_lin_mV,2)/(2*blockSize));
  scaling_lin_mV = pow(10,(double)scaling/100/20);
  return 10*log10(power_sum/(2*blockSize))+20*log10(scaling_lin_mV);
}


int
main (int argc, char *argv[], char *envp[])
{
  int ErrorCode;
  ExtractOptions options;
  Util util;

  // These are all necessary to open the stream file
  unsigned char StreamID = 0;
  TSMW_IQIF_STREAM_INFO_t StreamInfo;
  TSMW_IQIF_MEAS_CTRL_t MeasCtrl;
  TSMW_IQIF_CH_CTRL_t ChannelCtrl1;
  TSMW_IQIF_CH_CTRL_t ChannelCtrl2;
  TSMW_IQIF_FILTER_PARAM_t FilterSpec;
  long Coeff;
  unsigned long NoOfCoeffs = 1;

  options.parseCmd (argc,argv);

  util.loadK1Interface ();

  ErrorCode = TSMWIQOpenStreamFile_c ((char*)options.iq_file_location.c_str (), StreamID,
                                      &StreamInfo, &MeasCtrl,
                                      &ChannelCtrl1, &ChannelCtrl2,
                                      &FilterSpec, &Coeff, NoOfCoeffs);
  if (ErrorCode != 0) {
    std::cerr << "Make sure you are using a path starting with c: (or d:)" << std::endl;
    util.printLastError (ErrorCode);
    util.releaseK1Interface ();
    exit (-1);
  }

  // Now that the file is open, extract measurement parameters and
  // read file
  std::cout << "File opened: " << StreamInfo.NoOfBlocks << " blocks (skipped "
            << StreamInfo.NoOfSkippedBlocks << "), block size "
            << StreamInfo.BlockSize << std::endl;
  std::cout << "File description: " << StreamInfo.Description << std::endl;
  std::cout << "Number of channels on FE1: " << (unsigned int)ChannelCtrl1.NoOfChannels << std::endl;
  std::cout << "Number of channels on FE2: " << (unsigned int)ChannelCtrl2.NoOfChannels << std::endl;
  std::cout << "Splitter used: " << (unsigned int)MeasCtrl.Splitter << std::endl;

  // Prepare data-structures to extract stream data
  TSMW_IQIF_RESULT_t IQResult;
  unsigned int blockSize = StreamInfo.BlockSize;
  unsigned int numberOfChannels = (unsigned int)ChannelCtrl1.NoOfChannels
    + (unsigned int)ChannelCtrl2.NoOfChannels;
  double* pReal = (double*) malloc (numberOfChannels*blockSize*sizeof(double));
  double* pImag = (double*) malloc (numberOfChannels*blockSize*sizeof(double));
  short* pScaling = (short*) malloc (numberOfChannels*sizeof(short));
  unsigned long* pOverflow = (unsigned long*) malloc (numberOfChannels*sizeof(unsigned long));
  unsigned int *pCalibrated = (unsigned int*) malloc (numberOfChannels*sizeof(unsigned int));
  unsigned __int64 offset = 0;

  unsigned int timeOut = 10000;
  unsigned int channelOffset = 0;
  for (unsigned int countBlocks = 0; countBlocks < StreamInfo.NoOfBlocks; countBlocks++) {
    offset = countBlocks * blockSize;
    ErrorCode = TSMWIQGetStreamDouble_c (StreamID, timeOut, &IQResult,
                                         pReal, pImag, pScaling, pOverflow,
                                         pCalibrated, offset,
                                         blockSize, numberOfChannels);
    if (ErrorCode != 0) {
      util.printLastError (ErrorCode);
      util.releaseK1Interface ();
      exit (-1);
    }
    std::cout << "Fetched block " << countBlocks << std::endl;
    // Visual validation
    for (unsigned int countChannels = 0; countChannels < numberOfChannels; countChannels++) {
      channelOffset = countChannels*blockSize;
      std::cout << "Channel " << countChannels 
                << ": scaling/real/imag " << pScaling[countChannels] << " "
                << pReal[channelOffset] << " " << pImag[channelOffset] << std::endl;

      std::cout << "Channel " << countChannels << ": IQ power 1st sample "
                << util.get_iq_power (pScaling[countChannels], pReal[channelOffset], pImag[channelOffset]) << std::endl;

      std::cout << "Channel " << countChannels << ": avg. IQ power "
                << util.get_average_iq_power (pScaling[countChannels],
                                              pReal, pImag,
                                              channelOffset,
                                              blockSize)
                << std::endl;

    }
  }


  ErrorCode = TSMWIQCloseStreamFile_c (StreamID);
  if (ErrorCode != 0) {
    util.printLastError (ErrorCode);
    util.releaseK1Interface ();
    exit (-1);
  }


  util.releaseK1Interface ();
  return (0);
}
