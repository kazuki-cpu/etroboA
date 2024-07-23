//by mori 23/07/18
#include "app.h"
#include "util.h"
#include "Clock.h"
#include "Walker.h"
#include "Log.h"
#include <cmath>
#include "MathFnc.h"
#include "ArmCtrl.h"
#include "DistanceMonit.h"
using namespace ev3api;
using namespace std;
#define cycle_time 100*1000//100*1ms
static bool lastpress=false;
// ログNo.
unsigned int log_cyc_times = 0;
static bool endtrg=false;
Clock clock;
static ColorMonit colormonit;
static DistanceMonitor distancemonit;
static Walker walker;//class def
static ArmCtrl armctrl;
sCoordinate coonow;
enum eWalktype walktype;

   
/*main******************************************/
void main_task(intptr_t unused){
for (;;){
  endtrg = false;
  sta_cyc(CYC_LOG);//周期ハンドラ起動
  sta_cyc(CYC_WALK);
  sta_cyc(CYC_COLOR);
  //sta_cyc(CYC_SONAR);
  
  for (;;){
    
    clock.sleep(cycle_time);
    if (endtrg){break;}
  }
  
  stp_cyc(CYC_LOG);//周期ハンドラ停止
  stp_cyc(CYC_WALK);
  stp_cyc(CYC_COLOR);
  //stp_cyc(CYC_SONAR);
}
  
  ext_tsk();
}
/*distance*****************************************/
void sonar_task(intptr_t exinf){
  static bool inited=false;
  if (!inited){
    inited=true;
    return ;
  }
  distancemonit.cycle();
  ext_tsk();
}

/*color*****************************************/
void color_task(intptr_t exinf){
  static bool inited=false;
  if (!inited){
    inited=armctrl.init();
    return ;
  }
  colormonit.cycle();
  armctrl.cycle();
  ext_tsk();
}
/*walk*****************************************/
void walk_task(intptr_t exinf){
  static bool inited=false;
  static int walk_phase=-1;
  static int walk_phase_last=-2;
  enum eWorksta wksta_now;
  sCoordinate coo_now=walker.getCoordinate();
  if (!inited){
    walker.init();
    walk_phase=-1;
    inited=true;
  }
  wksta_now=walker.cycle(colormonit.getColor());
  if (ev3_button_is_pressed(LEFT_BUTTON)&&lastpress==false){
    lastpress=true;
    //walker.setWalkto(100,100);
    walk_phase=0;
    //walker.setTurnpivot(MathFnc::deg2rad(90),1,20);
  }
  else if (ev3_button_is_pressed(RIGHT_BUTTON)&&lastpress==false){
    lastpress=true;
    //walker.setTurnsping(MathFnc::deg2rad(-120),0);
    //walker.setWalk(-1,70,300.0);
    // walker.setWalk(1,70,300.0,1);
    walk_phase=0;

  }
  else if (ev3_button_is_pressed(UP_BUTTON)&&lastpress==false){
    lastpress=true;
    walker.setWalk(0,70,0);//MathFnc::deg2rad(-120),0);
    //walker.setWalk(-1,70,300.0);
    //walker.setWalk(1,70,300.0,1);
  }
  else if (ev3_button_is_pressed(DOWN_BUTTON)){
    lastpress=false;
    //walker.setTurnsping(MathFnc::deg2rad(-60),0);
    //walker.setWalk(-1,70,300.0);
    walker.setStop();
  }
  else {
    lastpress=false;
  }
  if (walk_phase==0){
    static int i=0;
    i++;
    if (i>3){
      i=0;walk_phase=1;
    }
  }
  if (walk_phase==1){
    static bool i=false;
    if (coo_now.y>=3370-300&&i){walk_phase=2;i=false;sprintf(execlog.logstr,"phase2");    //loghead
    execlog.log_tsk();}
    if (walk_phase_last!=1){
      walker.setWalk(0,90,0,-1);
      //walker.setWalkto(400,300);
      i=true;
    }
  }
  if (walk_phase==2){
    static bool i=false;
    if (coo_now.y>=3370-210&&i){walk_phase=3;i=false;sprintf(execlog.logstr,"phase3");    //loghead
    execlog.log_tsk();}
    if (walk_phase_last!=2){
      walker.setWalk(0,60,0,-1);
      //walker.setWalkto(400,300);
      i=true;
    }
  }
  if (walk_phase==3){
    static bool i=false;
    if (coo_now.th > PI &&i){walk_phase=4;i=false;    sprintf(execlog.logstr,"phase4");    //loghead
    execlog.log_tsk();}
    if (walk_phase_last!=3){
      walker.setWalk(1,60,210,0);
      //walker.setWalkto(400,300);
      i=true;
    }
  }
  if (walk_phase==4){
    static bool i=false;
    if (walker.getWalkdist()>=500&&i){walk_phase=0xFF;i=false;}
    if (walk_phase_last!=4){
      walker.setWalk(0,60,0,-1);
      //walker.setWalkto(400,300);
      i=true;
    }
  }
  if (walk_phase==0xFF){
    static bool i=false;
    walker.setStop();
    endtrg==true;
    inited=false;
  }
  
  walk_phase_last=walk_phase;
  ext_tsk();
}
/*log******************************************/
void log_task(intptr_t exinf) {
  static bool inited=false;
  if (!inited){
    execlog.startup();//ログファイルオープン
    sprintf(execlog.logstr,"cyctimes,");    //loghead
    execlog.log_tsk();
    sprintf(execlog.logstr,"x,y,th,walktype,");    //loghead
    execlog.log_tsk();
    sprintf(execlog.logstr,"PWM_L,PWM_R,");    //loghead
    execlog.log_tsk();
    sprintf(execlog.logstr,"R,G,B,bright,ColorName,");    //loghead
    execlog.log_tsk();
    sprintf(execlog.logstr,"Distance_flg,Distance,\n");    //loghead
    execlog.log_tsk();

    sprintf(execlog.logstr,"battery=");    //loghead
    execlog.log_tsk();
    int battery = ev3_battery_voltage_mV();// バッテリー値
    sprintf(execlog.logstr,"%d,\n",battery);
    execlog.log_tsk();

    
    inited=true;
  }
  if (endtrg){    
    execlog.finish();//ログファイルクローズ
  }

  log_cyc_times++;
  sCoordinate coo         = walker.getCoordinate();
  enum eWalktype walktype = walker.getWalktype();
  sPWMs pwm               = walker.getPWM();
  sColor color_now        = colormonit.getColor();
  sDistance distance_now = distancemonit.getDistance();

  sprintf(execlog.logstr,"%d,",log_cyc_times);
  execlog.log_tsk();
  sprintf(execlog.logstr,"%f,%f,%f,%d,",coo.x,coo.y,MathFnc::rad2deg(coo.th),walktype);
  execlog.log_tsk();
  sprintf(execlog.logstr,"%f,%f,",pwm.pwmL,pwm.pwmR);
  execlog.log_tsk();
  sprintf(execlog.logstr,"%d,%d,%d,%f,%d,",color_now.R,color_now.G,color_now.B,color_now.bright,color_now.name);
  execlog.log_tsk();
  sprintf(execlog.logstr,"%d,%d",distance_now.listen_flg,distance_now.Distance);
  execlog.log_tsk();
  sprintf(execlog.logstr,"\n");
  execlog.log_tsk();
  ext_tsk();
}
