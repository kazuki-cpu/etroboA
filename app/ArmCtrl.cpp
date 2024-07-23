#include "ArmCtrl.h"
#include "Log.h"
#include <cmath>
using namespace std;

ArmCtrl::ArmCtrl():
  Armmotor(PORT_A){}

void ArmCtrl::setAngle(int angle){
  this->angle_set=angle;
}
bool ArmCtrl::init(){
  static int i=0;
  Armmotor.setPWM(20);
  if (i<=67){
    i++;
  }
  else {
    return true;
  }
  
  if (i>=50){
    Armmotor.setPWM(0);
  }
  if (i==65){
    Armmotor.reset();
  }
  if (i==66){
    return true;
  }
  return false;
}
bool ArmCtrl::cycle (){
  angle_now=Armmotor.getCount();
  if (abs(angle_set-angle_now)<=2){
    return true;
  }
  if (angle_set>angle_now){
    Armmotor.setPWM(-60);
  }
  if (angle_set<angle_now){
    Armmotor.setPWM(+60);
  }
  return false;
}