#ifndef valdef_h
#define valdef_h
/*basic def*/

#define LOGSTRLEN 512 //ログファイル名サイズ
#define LOGFILENAME "001_1_3_08_1_3.csv" //ログファイル名
//COLOR_traceper_WALK_traceper_PID_default_kp_PID_default_ki_PID_default_kd_回数.csv

#define wheel_perimeter 102.0*PI //車輪周長mm
#define Wheel_dist 125.0 //車輪距離mm
#define COLOR_Midgray 70.0//ライントレース灰色中間値
#define COLOR_traceper 0.01
#define WALK_traceper 1.0

#define PWM_max 100
#define PWM_min -100

#define PWM_Lmax 1.0 /*左右の差が大きい場合はコレを下げる*/
#define PWM_Rmax 1.0

#define PID_default_kp 3.0
#define PID_default_ki 0.8
#define PID_default_kd 1.0

#define WALK_P 3.0
#define WALK_I 0.8
#define WALK_D 1.0
//#define pidlog
#define colortracelog
//#define pwmlog

#define SPING_P PID_default_kp
#define WALK_P PID_default_kp

/*fnc def*/
/*spingturn チューニング.jpgまでご覧ください*/
#define SPING_pwmmax 80 /*sping turn 時最大速度*/
#define SPING_USEPID 0.01 /*約4deg*/
#define SPING_PID_BREAK_CONDIT 0.01

/*pivotturn チューニング.jpgまでご覧ください*/
#define PIVOT_pwmmax 60 /*pivot turn 時最大速度*/
#define PIVOT_pwmmin 0 /*pivot turn 時最小速度*/


#define WALKTO_SPEED 90
#define WALKTO_WAITTIME1 30
#define WALKTO_WAITTIME2 40


#endif 