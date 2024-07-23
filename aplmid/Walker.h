#ifndef WALKER_H_
#define WALKER_H_

#include "util.h"
#include "Clock.h"

#include "valdef.h"
#include "ServoCtrl.h"
#include "ColorMonit.h"
using namespace ev3api;

enum eCtrace{
  ct_none=0,
  ct_courseL=1,
  ct_courseR=2,
};

enum eWorksta{
  ws_stop=0,
  ws_walk=1,
  ws_turn=2,
  ws_stopping=3
};
enum eWalktype{
  wt_stop=0,
  wt_straight=1,
  wt_curve=2,
  wt_sping=3,
  wt_pivot=4,
  wt_walkto=5
};

typedef struct
{
  double x;
  double y;
  double th;
} sCoordinate; /*座標*/

typedef struct 
{
  enum eWalktype walktype=wt_stop;
  double dist;
  double radius;
  double angle;
  int speed;
  int direction;
  int pivot_mode;
  //sCoordinate coo_target;
}sWalkSet; 
class Walker
{
//color_kp
#ifdef COLOR_P 
const double color_kp =COLOR_P;
#else 
const double color_kp =PID_default_kp;
#endif
//color_ki 
#ifdef COLOR_I
const double color_ki =COLOR_I;
#else 
const double color_ki =PID_default_ki;
#endif 
//color_kd
#ifdef COLOR_D
const double color_kd =COLOR_D;
#else 
const double color_kd =PID_default_kd;
#endif 
  
public:
  void init();
  enum eWorksta cycle (sColor color_read);// 周期関数10ms実行必須
  /*color tracer開閉*/
  void setColorTracer(enum eCtrace colortrace);
  
  /**/
  void setStop();

  
    /*(現在の絶対角度を初期角度にする)走行、但し：
    -R	半径
    -direction 旋回方向
    -speed	速度(0~100)
    -color	追跡色
  以下direction説明：
    if (direction ==1) 左折(反時計)　半径をRにして、円弧に走る、speedは左輪の速度。
    if (direction ==-1)右折(時計)　　半径をRにして、円弧に走る、speedは右輪の速度。
    if (direction ==0) 直進走る、speedは車輪速度。
  以下color説明:
    if (color ==1) L
    if (color ==-1) R
    if (color ==0) 補正しない。*/
  void setWalk(int direction,int speed,double R = 0,int usecolor = 0xFF);

  /*angle°まで超信地旋回,但し:
    -angle	反時計回りの角度
    -world	0:相対座標/1:絶対座標(世界座標)*/
  void setTurnsping(double angle, bool world);

  /*angle°まで信地旋回,但し:
    -angle	反時計回りの角度
    -world	0:相対座標/1:絶対座標(世界座標)
    -speed	動いているほうの車輪の速度
    -mode	  0:時計回り/1：反時計回り/-1:計算結果で判定*/
    
  void setTurnpivot(double angle, bool world, int speed, int mode=-1);

  /*世界座標まで直線走行、但し：
    -coo	到達座標
  *走行前、必ず直進方向に超信地旋回する。//(現在発動しない)走行後、必ずcoo.thまで超信地旋回する。*/
  void setWalkto(double coo_x,double coo_y);

  /*return世界座標*/
  sCoordinate getCoordinate();

  /*setWalk() OR setWalkto() OR setOffset()指令受けてから走った距離(mm) 円弧だと走った弧長*/
  double getWalkdist();

  /*setWalk() OR setWalkto() OR setOffset()現在のと指令受けた瞬間のRotateの差*/
  sServoRotate getWalkRotate();

  /*return 現在のwalktype*/
  eWalktype getWalktype();

  /*return 現在のpwm*/
  sPWMs getPWM();

  /*walkerが思ったセンサー色　今後戻り値を構造体にする*/
  enum eColorname getColor();


private:
  sColor color_now;
  sPWMs pwm_walker;
  sWalkSet walkset;//目標値
  bool needinit_walk=false;
  bool needinit_turn=false;
  bool needinit_walkto=false;
  sCoordinate coo_now;
  ServoCtrl servoctrl;
  sServoRotate walk_rotate_offset; // 指令の瞬間の値を記録する。
  int colortrace=0;
  double MVn_color;//the output of pid_color
  PIDFnc pid_color= PIDFnc(color_kp,color_ki,color_kd);
/*setfnc()に使用するoffset*/
  void setOffset();
/*座標記録*/
  sCoordinate cycle_coordinate();

/*指令実行部*/
  eWorksta cycle_stop(); 
  eWorksta cycle_walkstraight();
  eWorksta cycle_walkcurve();
  eWorksta cycle_turnsping();
  eWorksta cycle_walkto();
  eWorksta cycle_turnpivot();
/*ColorTrace補正部*/
  double colorPIDcorrection(double en);


};

#endif