#include "util.h"

#include <iostream>
#include <cmath>
#include <ctime>

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

unsigned int
Util::getNumberOfChannels (TSMW_IQIF_CH_CTRL_t *pChannelCtrl1,
			   TSMW_IQIF_CH_CTRL_t *pChannelCtrl2)
{
  unsigned int NoOfChannels = 0;

  if (pChannelCtrl1 == NULL) {
    // Frontend 2 used only
    NoOfChannels = pChannelCtrl2->NoOfChannels;
    std::cout << "FE2 freq: " << pChannelCtrl2->Frequency << std::endl;
  } else if (pChannelCtrl2 == NULL) {
    // Frontend 1 used only
    NoOfChannels = pChannelCtrl1->NoOfChannels;
    std::cout << "FE1 freq: " << pChannelCtrl1->Frequency << std::endl;
  } else {
    // Both frontends used
    NoOfChannels = pChannelCtrl1->NoOfChannels + pChannelCtrl2->NoOfChannels;
    std::cout << "FE1 freq: " << pChannelCtrl1->Frequency << std::endl;
    std::cout << "FE2 freq: " << pChannelCtrl2->Frequency << std::endl;
  }
  std::cout << "Total number of channels: " << NoOfChannels << "\n";

  return NoOfChannels;
}

void
Util::kill (int ErrorCode)
{
  printLastError (ErrorCode);
  releaseK1Interface ();
  exit (-1);
}

void
Util::waitForFrontendSync ()
{
    // Wait two seconds after connection establishment for frontend synchronization.
    std::cout << "Wait 2 seconds for frontend synchronization\n";
    clock_t trigger, seconds = 2;
    trigger = seconds * CLOCKS_PER_SEC + clock ();
    while (trigger > clock ());
}

double
Util::get_iq_power (short scaling, double real, double imag)
{
  double scaling_lin_mV;

  // See TSMWIQPlotData.m: 2000 is for 100 * 20 (voltage)
  scaling_lin_mV = std::pow(10,(double)scaling/100/20);
  /*
   * // Factor of 2 is because we assume a CW (sine/cosine) is sent.
   * return 10*std::log10((std::pow(real*scaling_lin_mV,2) + std::pow(imag*scaling_lin_mV,2))/2);
  */
  return 10*std::log10(std::pow(real*scaling_lin_mV,2) + std::pow(imag*scaling_lin_mV,2));
}

double
Util::get_average_iq_power (short scaling, double* real, double* imag,
                            unsigned int blockSize)
{
  double scaling_lin_mV;
  double power_sum = 0;

  for (unsigned int k = 0; k < blockSize; k++ ) {
    power_sum = power_sum +
      std::pow(real[k],2) +
      std::pow(imag[k],2);
  }
  // return 10*log10(power_sum*pow(scaling_lin_mV,2)/(2*blockSize));
  scaling_lin_mV = std::pow(10,(double)scaling/100/20);
  // return 10*std::log10(power_sum/(2*blockSize))+20*std::log10(scaling_lin_mV);
  return 10*std::log10(power_sum/(blockSize))+20*std::log10(scaling_lin_mV);
}
