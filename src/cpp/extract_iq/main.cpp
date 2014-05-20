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

#include <cmath>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <stdio.h>

#include <tchar.h>

#include "../common/util.h"

class ExtractOptions
{
private:

public:
  ExtractOptions (void) : limitExtractedBlock (0),
                          saveToFile (false)
  {}

  void parseCmd (int, char **);
  void printUsage (char* program);

  unsigned int limitExtractedBlock;
  bool saveToFile;
  std::string iq_file_location;
};

void
ExtractOptions::printUsage (char* program)
{
  std::cout << program << " [-s|-n INT|-h|--help] filename\n\n"
            << "\t-s\t\tSave stream data to text file (inactive by default)\n"
            << "\t-n INT\t\tNumber of blocks to extract (default is all)\n"
            << "\t-h|--help\tDisplay this help message"
            << std::endl;
}

void
ExtractOptions::parseCmd (int argc, char *argv[])
{
  // Display each command-line argument
  if (argc == 1) {
    printUsage (argv[0]);
    exit (0);
  }

  bool valid = false;
  std::string help ("--help");
  std::string short_help ("-h");
  std::string blocks ("-n");
  std::string print ("-s");

  for (int count = 1; count < argc; count++) {
    valid = false;
    if (help.compare (argv[count]) == 0 || short_help.compare (argv[count]) == 0) {
      printUsage (argv[0]);
      exit (0);
    }
    if (blocks.compare (argv[count]) == 0) {
      assert (argc >= count+1);
      count++;
      limitExtractedBlock = atoi (argv[count]);
      valid = true;
    }
    if (print.compare (argv[count]) == 0) {
      saveToFile = true;
      valid = true;
    }
    // Last option: must be the file location of the file to extract
    if (count+1 >= argc) {
      iq_file_location.assign(argv[count]);
      valid = true;
    }
    if (valid == false) {
      std::cerr << "Invalid option or option specification: " << argv[count] << std::endl;
      printUsage (argv[0]);
      exit (-1);
    }
  }
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

  std::vector<FILE *> textIQFiles (numberOfChannels);
  if (options.saveToFile) {
    for (unsigned int k = 0; k < textIQFiles.size (); k++) {
      std::string filename = options.iq_file_location +
        std::string ("_channel_") + static_cast<std::ostringstream*>( &(std::ostringstream() << k) )->str() + std::string(".csv");
      std::cout << filename << std::endl;
      textIQFiles[k] = fopen (filename.c_str (), "w");

    }
  }



  unsigned int timeOut = 10000;
  unsigned int channelOffset = 0;
  unsigned int numberOfBlocks = StreamInfo.NoOfBlocks;
  if (options.limitExtractedBlock > 0)
    numberOfBlocks = options.limitExtractedBlock;
  for (unsigned int countBlocks = 0; countBlocks < numberOfBlocks; countBlocks++) {
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
    std::cout << "Fetched block " << countBlocks << ": sampling rate "
              << IQResult.Fsample << std::endl;

    // Visual validation
    for (unsigned int countChannels = 0; countChannels < numberOfChannels; countChannels++) {
      channelOffset = countChannels*blockSize;
      /* 
      std::cout << "Channel " << countChannels 
                << ": scaling/real/imag " << pScaling[countChannels] << " "
                << pReal[channelOffset] << " " << pImag[channelOffset] << std::endl;

      std::cout << "Channel " << countChannels << ": IQ power 1st sample "
                << util.get_iq_power (pScaling[countChannels], pReal[channelOffset], pImag[channelOffset]) << std::endl;
      */
      std::cout << "Channel " << countChannels << ": avg. IQ power "
                << util.get_average_iq_power (pScaling[countChannels],
                                              &pReal[channelOffset], &pImag[channelOffset],
                                              blockSize)
                << std::endl;

      if (options.saveToFile) {
        for (unsigned int k = 0; k < blockSize; k++) {
          fprintf (textIQFiles[countChannels], "%f;%hd;%d;%d\n",
                   IQResult.Fsample, pScaling[countChannels],
                   (int)pReal[channelOffset+k],(int)pImag[channelOffset+k]);
        }
      }
    }
  }

  if (options.saveToFile) {
    for (unsigned int k = 0; k < textIQFiles.size (); k++) {
      fclose (textIQFiles[k]);
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
