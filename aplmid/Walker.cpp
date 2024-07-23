#include "Walker.h"//self header

#include <math.h>
#include "MathFnc.h"
#include "Log.h"
using namespace std;
//using namespace mathFnc;

enum eWorksta Walker::cycle(sColor color_read){
  enum eWorksta workstatus;
  color_now=color_read;
  cycle_coordinate();
  
  switch (walkset.walktype){
    case wt_stop:
      workstatus=cycle_stop();
      break;
    case wt_straight:
      workstatus=cycle_walkstraight();
      break;
    case wt_curve:
      workstatus=cycle_walkcurve();
      break;
    case wt_sping:
      workstatus=cycle_turnsping();
      break;
    case wt_pivot:
      workstatus=cycle_turnpivot();
      break;
    case wt_walkto:
      workstatus=cycle_walkto();
      break;
    default:
      workstatus=cycle_stop();
      break;
  }
  
  if (workstatus==ws_stop||workstatus==ws_stopping){
    walkset.walktype=wt_stop;
  }
  else {
    servoctrl.setPWM(pwm_walker.pwmL,pwm_walker.pwmR);
  }
  return workstatus;
}

void Walker::init(){
  init_f(__FILE__);
  servoctrl.setReset(allservo);
  coo_now.th=0;
  coo_now.x=0;
  coo_now.y=200;
}
void Walker::setColorTracer(enum eCtrace colortrace_set){
  colortrace=colortrace_set;
  if (colortrace_set==ct_none){
    pid_color.clean();
  }
}
void Walker::setStop(){
  walkset.walktype=wt_stop;
//write log
  sprintf(execlog.logstr,"SetStop(),\n");
  execlog.log_tsk();
}
void Walker::setWalk(int direction,int speed,double R,int usecolor){
  needinit_walk=true;
//mode select
  walkset.speed=speed;
  walkset.radius=R;
  walkset.direction=direction;
  if (direction==0){
    walkset.walktype=wt_straight;
  }
  else {
    walkset.walktype=wt_curve;
  }

  if (usecolor==-1){
    setColorTracer(ct_courseR);
  }
  else if (usecolor==1){
    setColorTracer(ct_courseL);
  }
  else if (usecolor==0){
    setColorTracer(ct_none);
  }
//write log
  sprintf(execlog.logstr,"Setwalk(direction/R/speed/),");
  execlog.log_tsk();
  sprintf(execlog.logstr,"%d,%f,%d,\n",direction,R,speed);
  execlog.log_tsk();
}
void Walker::setTurnsping(double angle, bool world){
  needinit_turn=true;
//do things
  walkset.walktype=wt_sping;
  if (world){
    walkset.angle=MathFnc::rad1round(angle-coo_now.th);
    //walkset.angle=(angle-coo_now.th);
  }
  else {
    walkset.angle=angle;
  }
//write log
  sprintf(execlog.logstr,"setSpingturn(angle/world),");
  execlog.log_tsk();
  sprintf(execlog.logstr,"%f,%d,\n",angle,world);
  execlog.log_tsk();
}
void Walker::setTurnpivot(double angle,bool world,int speed,int mode){
  needinit_turn=true;
//do things
  walkset.walktype=wt_pivot;
  if (world){
    walkset.angle=MathFnc::rad1round(angle-coo_now.th);
  }
  else {
    walkset.angle=angle;
  }
  walkset.pivot_mode = mode;
//write log
  sprintf(execlog.logstr,"setPivotturn(angle/world/speed/mode),");
  execlog.log_tsk();
  sprintf(execlog.logstr,"%f,%d,%d,%d,\n",walkset.angle,world,speed,walkset.pivot_mode);
  execlog.log_tsk();
}

void Walker::setWalkto(double coo_x,double coo_y){
  needinit_walkto=true;
//do things
  walkset.walktype=wt_walkto;
  double th_want=atan2((coo_y-coo_now.y),(coo_x-coo_now.x));
  walkset.angle=MathFnc::rad1round(th_want-coo_now.th);
  walkset.dist=sqrt(pow((coo_x-coo_now.x),2)+pow((coo_y-coo_now.y),2));
  //write log
  sprintf(execlog.logstr,"walkto_thwant=");
  execlog.log_tsk();
  sprintf(execlog.logstr,"%f,",th_want);
  execlog.log_tsk();
  sprintf(execlog.logstr,"coo_now.th=");
  execlog.log_tsk();
  sprintf(execlog.logstr,"%f,",coo_now.th);
  execlog.log_tsk();
  sprintf(execlog.logstr,"walkset.angle=");
  execlog.log_tsk();
  sprintf(execlog.logstr,"%f,\n",walkset.angle);
  execlog.log_tsk();
  //write log
  sprintf(execlog.logstr,"setWalkto(x/y),");
  execlog.log_tsk();
  sprintf(execlog.logstr,"%f,%f,\n",coo_x,coo_y);
  execlog.log_tsk();
}

sCoordinate Walker::getCoordinate(){
  return coo_now;
}
double Walker::getWalkdist(){
  double ans;
  sServoRotate now;
  now=servoctrl.getRotate();
  ans=(now.leftDistance-walk_rotate_offset.leftDistance)+(now.rightDistance-walk_rotate_offset.rightDistance)/2.0;
  return ans;
}

sServoRotate Walker::getWalkRotate(){
  sServoRotate now;
  now=servoctrl.getRotate();
  now.leftAngle-=walk_rotate_offset.leftAngle;
  now.rightAngle-=walk_rotate_offset.rightAngle;
  now.leftDistance-=walk_rotate_offset.leftDistance;
  now.rightDistance-=walk_rotate_offset.rightDistance;
  return now;
}
eWalktype Walker::getWalktype(){
  return walkset.walktype;
}
sPWMs Walker::getPWM(){
  return servoctrl.getPWM();
}
enum eColorname Walker::getColor(){
  return color_now.name;
}

/*private**************************************************************/
void Walker::setOffset(){
  walk_rotate_offset=servoctrl.getRotate();
}
sCoordinate Walker::cycle_coordinate(){
  //static sCoordinate coo;
  static sServoRotate rotate_last;
  sServoRotate rotate;
  double dR,dL;
  double dX,dY,dTH;
  //init
  static bool inited=false; 
  if (!inited){
    rotate_last=servoctrl.getRotate();
    coo_now.th=PI/2.0;
    inited=true;
  }

  rotate=servoctrl.getRotate();
  dR=rotate.leftDistance-rotate_last.leftDistance;
  dL=rotate.rightDistance-rotate_last.rightDistance;
  rotate_last=rotate;
  //calcu
  dX=cos(coo_now.th)*  (dR+dL)/2.0;
  dY=sin(coo_now.th)*  (dR+dL)/2.0;
  dTH=atan((dL-dR)/Wheel_dist);

  //write val
  coo_now.x+=dX;
  coo_now.y+=dY;
  coo_now.th+=dTH;
  return coo_now;
}

eWorksta Walker::cycle_stop(){
  sPWMs pwm_get;
  pwm_get=servoctrl.getPWM();
  
  if (pwm_get.pwmL||pwm_get.pwmR){
    pwm_walker.pwmL=0;
    pwm_walker.pwmR=0;
    servoctrl.setStop(allservo);
    return ws_stopping;
  }
  return ws_stop;
}
eWorksta Walker::cycle_walkstraight(){
//def////////////////////////////
//kp
#ifdef WALK_P
const double kp= WALK_P;
#else
const double kp= PID_default_kp;
#endif 
//ki
#ifdef WALK_I
const double ki= WALK_I;
#else
const double ki= PID_default_ki;
#endif 
//kd
#ifdef WALK_D
const double kd= WALK_D;
#else
const double kd= PID_default_kd;
#endif 
//...////////////////////////////
  static double speed=0;
  double left,right;
  sServoRotate rotate_now;
  PIDFnc pid(kp,ki,kd);
  double en;//pid input
  double MVn;//pid output
//init
  if (needinit_walk){
    setOffset();
    pid.clean();
    speed=walkset.speed;
    needinit_walk=false;
  }
  rotate_now=getWalkRotate();
  en=(rotate_now.leftDistance-rotate_now.rightDistance)*WALK_traceper;
  en=colorPIDcorrection(en);
/*
  sprintf(execlog.logstr,"leftDistance=");
  execlog.log_tsk();
  sprintf(execlog.logstr,"%f,\n",rotate_now.leftDistance);
  execlog.log_tsk();
  sprintf(execlog.logstr,"rightDistance=");
  execlog.log_tsk();
  sprintf(execlog.logstr,"%f,\n",rotate_now.rightDistance);
  execlog.log_tsk();
*/
#ifdef pidlog
  sprintf(execlog.logstr,"en=,");
  execlog.log_tsk();
  sprintf(execlog.logstr,"%f,\n",en);
  execlog.log_tsk();
#endif
  MVn=pid.cycle(en);
  left=speed;
  right=speed;
  if (MVn>0){
    left-=MVn;
  }
  else if (MVn<0){
    right+=MVn;
  }
  else {
    //donothing
  }
  pwm_walker.pwmL=left;
  pwm_walker.pwmR=right;
  return ws_walk;
}
eWorksta Walker::cycle_walkcurve(){
//def////////////////////////////
//kp
#ifdef WALK_P
const double kp= WALK_P;
#else
const double kp= PID_default_kp;
#endif 
//ki
#ifdef WALK_I
const double ki= WALK_I;
#else
const double ki= PID_default_ki;
#endif 
//kd
#ifdef WALK_D
const double kd= WALK_D;
#else
const double kd= PID_default_kd;
#endif 
//...////////////////////////////
  static int direction=0;
  static double R=0;
  static int speed=0;
  sServoRotate rotate_now;
  double pwmr;
  double pwml;
  PIDFnc pid(kp,ki,kd);
//init
  if (needinit_walk){
    setOffset();
    pid.clean();
    direction=walkset.direction;
    R=walkset.radius;
    speed=walkset.speed;
    needinit_walk=false;
  }

  rotate_now=getWalkRotate();
  if (direction>0){//反時計回りなのでR-fast 
    double en;//pid input
    double MVn;//pid output
    const double ratio=(R-(Wheel_dist/2.0))/(R+(Wheel_dist/2.0));
    en=(rotate_now.leftDistance-(rotate_now.rightDistance*ratio))*WALK_traceper;
    en=colorPIDcorrection(en);
#ifdef pidlog
    sprintf(execlog.logstr,"en=,");
    execlog.log_tsk();
    sprintf(execlog.logstr,"%f,\n",en);
    execlog.log_tsk();
#endif
    MVn=pid.cycle(en);
    pwmr=speed;
    pwml=ratio*speed-MVn;
  }
  else if (direction<0){//時計回りなのでL-fast 
    double en;//pid input
    double MVn=0;//pid output
    const double ratio=(R-(Wheel_dist/2.0))/(R+(Wheel_dist/2.0));
    // en=rotate_now.rightDistance-(rotate_now.leftDistance*ratio);
    en=(rotate_now.rightDistance-(rotate_now.leftDistance*ratio))*WALK_traceper;

    en=colorPIDcorrection(en);
#ifdef pidlog
    sprintf(execlog.logstr,"en=,");
    execlog.log_tsk();
    sprintf(execlog.logstr,"%f,\n",en);
    execlog.log_tsk();
#endif
    MVn=pid.cycle(en);
    pwml=speed;
    pwmr=ratio*speed-MVn;
  }
  else{
    return ws_stopping;
  }
  //if (pwml>speed){
  //  pwml=speed;
  //}
  //if (pwmr>speed){
  //  pwmr=speed;
  //}
  pwm_walker.pwmL=pwml;
  pwm_walker.pwmR=pwmr;
  return ws_walk;
}
eWorksta Walker::cycle_turnsping(){
const double pwmmax=SPING_pwmmax;

//def////////////////////////////
#ifdef SPING_PID_BREAK_CONDIT
const double pid_break_condit=SPING_PID_BREAK_CONDIT;
#else
const double pid_break_condit=0.10;
#endif
//kp
#ifdef SPING_P
const double kp= SPING_P;
#else
const double kp= PID_default_kp;
#endif 
//ki
#ifdef SPING_I
const double ki= SPING_I;
#else
const double ki= PID_default_ki;
#endif 
//kd
#ifdef SPING_D
const double kd= SPING_D;
#else
const double kd= PID_default_kd;
#endif 
//...////////////////////////////
  sPWMs pwm_set;
  double pwm_l;
  double pwm_r;
  static double d_set;//片方の車輪の回す距離(目標距離) AKA Spingturn弧長
  double d_remain_l;//目標距離までの残り距離L
  double d_remain_r;//目標距離までの残り距離R
  sServoRotate rotate_now;
  PIDFnc pid_l(kp,ki,kd);
  PIDFnc pid_r(kp,ki,kd);
  if (needinit_turn){
    setOffset();
    pid_l.clean();
    pid_r.clean();
    d_set=(walkset.angle)*Wheel_dist/2.0;
    needinit_turn=false;
  }
  rotate_now=getWalkRotate();
  if (d_set>0){//反時計回り旋回
    d_remain_l=d_set+rotate_now.leftDistance;
    d_remain_r=d_set-rotate_now.rightDistance;
    pwm_l=pid_l.cycle(d_remain_l);
    if (pwm_l>pwmmax){
      pwm_l=pwmmax;
    }
    pwm_r=pid_r.cycle(d_remain_r);
    if (pwm_r>pwmmax){
      pwm_r=pwmmax;
    }
    pwm_walker.pwmL=-pwm_l;
    pwm_walker.pwmR=pwm_r;
  }
  else {//時計回り旋回
    d_remain_l=-d_set-rotate_now.leftDistance;
    d_remain_r=-d_set+rotate_now.rightDistance;
    pwm_l=pid_l.cycle(d_remain_l);
    if (pwm_l>pwmmax){
      pwm_l=pwmmax;
    }
    pwm_r=pid_r.cycle(d_remain_r);
    if (pwm_r>pwmmax){
      pwm_r=pwmmax;
    }
    pwm_walker.pwmL=pwm_l;
    pwm_walker.pwmR=-pwm_r;
  }
  double lmt=(SPING_USEPID*Wheel_dist/2.0/PI);
  if (lmt>d_remain_l&&lmt>d_remain_r&&pwm_l<pid_break_condit&&pwm_r<pid_break_condit){
    pwm_walker.pwmL=0;
    pwm_walker.pwmR=0;
    servoctrl.setStop(allservo);
    return ws_stopping;
  }
  return ws_turn;

}

eWorksta Walker::cycle_turnpivot(){
const double pwmmax=PIVOT_pwmmax;
const double pwmmin=PIVOT_pwmmin;

//def////////////////////////////
//今は切り替えない
const double pid_break_condit=SPING_PID_BREAK_CONDIT;
const double kp= PID_default_kp;
const double ki= PID_default_ki;
const double kd= PID_default_kd;
//...////////////////////////////
  sPWMs pwm_set;
  double pwm_pivot;
  static double d_set;//片方の車輪の回す距離(目標距離) AKA Spingturn弧長
  double d_remain;//目標距離までの残り距離
  sServoRotate rotate_now;
  PIDFnc pid_pivot(kp,ki,kd);
  // const double lmt = MathFnc::abs(walkset.angle*Wheel_dist);//円弧を算出

  if (needinit_turn){
    setOffset();
    pid_pivot.clean();
    d_set=MathFnc::abs((walkset.angle)*Wheel_dist);
    if (walkset.pivot_mode==-1 && walkset.angle>0){
      walkset.pivot_mode = 1;
    }
    else if(walkset.pivot_mode==-1 && walkset.angle<0) {
      walkset.pivot_mode = 0;
    }
    needinit_turn=false;
  }
  rotate_now=getWalkRotate();
  if (walkset.pivot_mode==1){//反時計回り旋回
    d_remain=d_set-rotate_now.rightDistance;
    pwm_pivot=pid_pivot.cycle(d_remain);
    if (pwm_pivot>pwmmax){
      pwm_pivot=pwmmax;
    }
    pwm_walker.pwmL=0;
    pwm_walker.pwmR=pwm_pivot;
  }
  else if(walkset.pivot_mode==0) {//時計回り旋回
    d_remain=d_set-rotate_now.leftDistance;
    pwm_pivot=pid_pivot.cycle(d_remain);
    if (pwm_pivot>pwmmax){
      pwm_pivot=pwmmax;
    }
    pwm_walker.pwmL=pwm_pivot;
    pwm_walker.pwmR=0;
  }
  //write log
  sprintf(execlog.logstr,"cyclePivotturn(angle/world/speed/mode),");
  execlog.log_tsk();
  sprintf(execlog.logstr,"%f,%f,%f,%d,\n",walkset.angle,d_remain,d_set,walkset.pivot_mode);
  execlog.log_tsk();
  double lmt=SPING_USEPID*d_set;
  if (lmt>d_remain&&pwm_pivot<pid_break_condit){
    pwm_walker.pwmL=0;
    pwm_walker.pwmR=0;
    servoctrl.setStop(allservo);
    return ws_stopping;
  }
  return ws_turn;
}
eWorksta Walker::cycle_walkto(){
  #define walktospeed WALKTO_SPEED
  #define walktowait1 WALKTO_WAITTIME1
  #define walktowait2 WALKTO_WAITTIME2
  static int phase=0;
  static bool phase_needinit=false;
  static sCoordinate coo_tar;
  static double dist_set;
  eWorksta wksta_now=ws_stop;
  //sCoordinate coo_begin;
  wksta_now=ws_stop;
  if (needinit_walkto){
    dist_set=walkset.dist;
    phase=0;
    phase_needinit=true;
    //coo_tar=walkset.coo_target;//not use fornow
    needinit_walkto=false;
  }
  if (phase==0){
    wksta_now= ws_turn;
    if (phase_needinit){
      this->needinit_turn=true;
      phase_needinit=false;
    }
    eWorksta wkst=cycle_turnsping();
    if (wkst==ws_stopping||wkst==ws_stop){
      phase=1;phase_needinit=true;
    }
  }
  else if (phase==1){//make sure stop after sping turn
    wksta_now= ws_turn;
    static int i=0;
    i++;
    cycle_stop();
    if (i>=walktowait1){
      phase=2;i=0;
    }

  }
  else if (phase==2){
    wksta_now= ws_walk;
    if (phase_needinit){
      this->needinit_walk=true;
      walkset.speed=walktospeed;
      phase_needinit=false;
    }
    cycle_walkstraight();
    double dist_now=getWalkdist();
  
    if (dist_now>=dist_set*0.99){
      //servoctrl.setStop(allservo);
      phase=3;phase_needinit=true;}
  }
  else if (phase==3){
    wksta_now= ws_walk;
    static int i=0;
    i++;
    cycle_stop();
    if (i>=walktowait2){
      phase=4;i=0;
    }

  }
  sprintf(execlog.logstr,"phase=,");
  execlog.log_tsk();
  sprintf(execlog.logstr,"%d,\n",phase);
  execlog.log_tsk();
  return wksta_now;
}

double Walker::colorPIDcorrection(double en){
  if (colortrace==ct_courseR){
    en-=(color_now.bright)*COLOR_traceper;
  }
  else if (colortrace==ct_courseL){
    en+=(color_now.bright)*COLOR_traceper;
  }
  return en;
}