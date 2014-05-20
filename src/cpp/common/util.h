#include "../include/TSMWIQInterfaceFunc.h"
#include "../include/TSMWIQInterfaceTypes.h"

class Util
{
public:
  void printLastError (int);
  void waitForFrontendSync ();
  void loadK1Interface ();
  void releaseK1Interface ();
  void kill (int);

  double get_iq_power (short scaling, double, double);
  double get_average_iq_power (short, double*, double*,
                               unsigned int);
};
