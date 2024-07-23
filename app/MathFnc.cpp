#include "MathFnc.h"
double MathFnc::abs (double in){
  if (in>=0){return in;}
  else {return -in;}
}
double MathFnc::rad2deg(double rad){
  return rad*360.0/(2.0*PI);
}
double MathFnc::deg2rad(double deg){
  return deg*(2.0*PI)/360.0;
}
double MathFnc::rad1round(double rad){
  for (;;){
    if (rad>PI){
      rad=rad-2*PI;
    }
    else if (rad<=(-PI)){
      rad=rad+2*PI;
    }
    else {
      break;
    }
  }
  return rad;
}

/*PID**********************/
PIDFnc::PIDFnc(double init_kp,double init_ki,double init_kd){
  kp=init_kp;
  ki=init_ki;
  kd=init_kd;
  MVn=0;
  MVn_last=0;
  MVn_delta=0;
  en=0;
  en_1=0;
  en_2=0;
}
PIDFnc::PIDFnc(){
  kp=PID_default_kp;
  ki=PID_default_ki;
  kd=PID_default_kd;
  MVn=0;
  MVn_last=0;
  MVn_delta=0;
  en=0;
  en_1=0;
  en_2=0;
}
double PIDFnc::cycle(double en){
//calcu
  MVn_delta=kp*(en-en_1)+ki*en - kd*((en-en_1)-(en_1-en_2));
  MVn=MVn_last + MVn_delta;

//remember
  MVn_last=MVn;
  en_2=en_1;
  en_1=en;
  return MVn;
}
/*
記号で表すと
　MVn = MVn_1 + ΔMVn
　ΔMVn = Kp(en-en_1) + Ki en + Kd((en-en_1) - (en_1-en_2))

MVn、MVn-1:今回、前回操作量　　ΔMVn:今回操作量差分
en,en-1,en-2：今回、前回、前々回の偏差
*/
void PIDFnc::clean(){
  MVn=0;
  MVn_last=0;
  MVn_delta=0;
  en=0;
  en_1=0;
  en_2=0;
}