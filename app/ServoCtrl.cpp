#include "ServoCtrl.h"
#include "Log.h"

ServoCtrl::ServoCtrl():
  Lservo(PORT_C),Rservo(PORT_B){}

void ServoCtrl::setPWM(double pwmL,double pwmR){
  Lservo.setPWM((int)(pwmL*PWM_Lmax));
  Rservo.setPWM((int)(pwmR*PWM_Rmax));
  pwms.pwmL=pwmL;
  pwms.pwmR=pwmR;
#ifdef pwmlog
  sprintf(execlog.logstr,"pwms,");
  execlog.log_tsk();
  sprintf(execlog.logstr,"%f,%f,\n",pwmL,pwmR);
  execlog.log_tsk();
#endif
}
void ServoCtrl::setStop(int servonum){
  if (servonum==leftservo){
    Lservo.stop();
  }
  else if (servonum==rightservo){
    Rservo.stop();
  }
  else {
    Lservo.stop();
    Rservo.stop();
  }
}
void ServoCtrl::setReset(int servonum){
  if (servonum==leftservo){
    Lservo.reset();
  }
  else if (servonum==rightservo){
    Rservo.reset();
  }
  else {
    Lservo.reset();
    Rservo.reset();
  }
}
void ServoCtrl::setInitRotate(){
  Lservo.reset();
  Rservo.reset();
  round[0]=0;
  round[1]=0;
  //servoRotate={};
}
sPWMs ServoCtrl::getPWM(){
  return pwms;
}
sServoRotate ServoCtrl::getRotate(){
  sServoRotate rotate;
  rotate.leftAngle=Lservo.getCount();
  rotate.rightAngle=Rservo.getCount();
  rotate.leftDistance =rotate.leftAngle *wheel_perimeter /360.0;
  rotate.rightDistance=rotate.rightAngle *wheel_perimeter /360.0;

  return rotate;
}