#include "valdef.h"
#include "Motor.h"
#include "MathFnc.h"
using namespace ev3api;

class ArmCtrl{

  public:
    ArmCtrl();
    bool cycle ();
    void setAngle(int angle);
    bool init();
  private:
    Motor Armmotor;
    int angle_set;
    int angle_now;
};
