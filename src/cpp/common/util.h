#include "../include/TSMWIQInterfaceFunc.h"
#include "../include/TSMWIQInterfaceTypes.h"

class Util
{
public:
  void printLastError (int);
  void loadK1Interface ();
  void releaseK1Interface ();

  double get_iq_power (short scaling, double, double);
  double get_average_iq_power (short, double*, double*,
                               unsigned int);
};
