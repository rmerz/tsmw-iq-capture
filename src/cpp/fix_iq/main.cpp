/**
 * TSMW Project  TSMW IQ helper: activates TSMQ to fix timing between
 *               both frontends
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

class CaptureOptions
{
private:

public:
  CaptureOptions (void) : verbose (false)
  {}

  void parseCmd (int, char **);

  bool verbose;
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
  std::string help ("--help");
  std::string short_help ("-h");
  std::string verbose_opt ("-v");


  for (int count = 1; count < argc; count++) {
    // std::cout << "  argv[" << count << "]   "
    //        << argv[count] << "\n";
    valid = false;
    if (help.compare (argv[count]) == 0 || short_help.compare (argv[count]) == 0) {
      std::cout << "-v\t\tVerbose mode\n"
                << "--help|-h\tPrints this help message\n" << std::endl;
      exit (0);
    }

    if (verbose_opt.compare (argv[count]) == 0) {
      verbose = true;
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

  // Initialize TSMW IQ Interface
  util.loadK1Interface ();

  // Connect to TSMW
  ErrorCode = TSMWConnect_c (IPAddress, &TSMWMode, &TSMWID);
  if (ErrorCode == 0) {

    printf ("Connected\n");
    util.waitForFrontendSync ();

    printf ("Now wait. Press any key to exit.\n");
    while (!_kbhit()) {
      Sleep (2000); // In milliseconds
    }

  } else {
    util.printLastError (ErrorCode);
  }

  util.releaseK1Interface ();

  return (0);
}
