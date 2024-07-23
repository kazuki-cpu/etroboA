#ifndef servoctrl_h
#define servoctrl_h

#include "valdef.h"
#include "Motor.h"
#include "MathFnc.h"
using namespace ev3api;

//servo
////////////////////////////////////////////////////////////////

typedef struct {
  int leftAngle=0;
  int rightAngle=0;
  double leftDistance=0;
  double rightDistance=0;
}sServoRotate;

typedef struct {
  double pwmL=0;
  double pwmR=0;
}sPWMs;

//////////////////////////
class ServoCtrl {
#define leftservo 1
#define rightservo 2
#define allservo 0


//#define cycle_per 1000 //10ms
  private:
  sPWMs pwms;

  public:
  ServoCtrl();
  void setPWM(double pwmL,double pwmR);
  void setReset(int servonum);
  void setStop(int servonum);
  void setInitRotate();
  sPWMs getPWM();
  sServoRotate getRotate();
  private:
  Motor Lservo;
  Motor Rservo;
  sServoRotate servoRotate;
  int round[2];

};

#endif 