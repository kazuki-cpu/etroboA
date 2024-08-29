#include "app.h"
#include "Calibration.h"
#include "Color.h"
#include <stdio.h>

static int16_t v_min = 255;
static int16_t v_max = 0;
static int16_t v_ave;
static int16_t s_min = 255;
static int16_t s_max = 0;
static int16_t s_ave;
static int16_t h_min = 255;
static int16_t h_max = 0;
static int16_t h_ave;

void calibration(bool_t flg){
    if(flg){
        s_calibration();                /* S値キャリブレーション（青線） */
        v_calibration();                /* V値キャリブレーション（黒線） */
        h_calibration();                /* H値キャリブレーション（黒線） */
    }else{
        skip_calibration();             /* キャリブレーションスキップ（固定値代入） */
    }
}

/* S値キャリブレーション関数 */
void s_calibration(){
    printf("S Calibration Start\n");
    #ifdef ETROBOC_SIM                      /* シミュレータのみ */
        /* 後退 */
        ev3_motor_set_power(left_motor,  -10);
        ev3_motor_set_power(right_motor, -10);
        wait_msec(1000);
        /* 停止 */
        ev3_motor_set_power(left_motor,  0);
        ev3_motor_set_power(right_motor, 0);
        wait_msec(500);
    #else
    #endif
    sta_cyc(S_CALIB_TASK_CYC);              /* 光センサV値キャリブレーションタスク起動 */
    calib_movement();                       /* キャリブレーション動作 */
    stp_cyc(S_CALIB_TASK_CYC);              /* 光センサV値キャリブレーションタスク停止 */
    color_set_s_calib(s_min, s_max, s_ave);
    printf("S Calibration Finished\n");
    #ifdef ETROBOC_SIM
        wait_msec(500);
    #else   /* RasPike実機 */
        wait_start();                           /* 動作開始待ち（実機：BlueToothボタンで開始） */
    #endif
}

/* V値キャリブレーション関数 */
void v_calibration(){
    printf("V Calibration Start\n");
    #ifdef ETROBOC_SIM                      /* シミュレータのみ */
        /* 前進 */
        ev3_motor_set_power(left_motor,  10);
        ev3_motor_set_power(right_motor, 10);
        wait_msec(1300);
        /* 停止 */
        ev3_motor_set_power(left_motor,  0);
        ev3_motor_set_power(right_motor, 0);
        wait_msec(500);
    #else
    #endif
    sta_cyc(V_CALIB_TASK_CYC);              /* 光センサV値キャリブレーションタスク起動 */
    calib_movement();                       /* キャリブレーション動作 */
    stp_cyc(V_CALIB_TASK_CYC);              /* 光センサV値キャリブレーションタスク停止 */
    color_set_v_calib(v_min, v_max, v_ave);
    printf("V Calibration Finished\n");
    #ifdef ETROBOC_SIM
        wait_msec(500);
    #else   /* RasPike実機 */
        wait_start();                       /* 動作開始待ち（実機：BlueToothボタンで開始） */
    #endif
}

/* H値キャリブレーション関数 */
void h_calibration(){
    printf("H Calibration Start\n");
    #ifdef ETROBOC_SIM                      /* シミュレータのみ */
        /* 後退 */
        ev3_motor_set_power(left_motor,  -10);
        ev3_motor_set_power(right_motor, -10);
        wait_msec(1000);
        /* 停止 */
        ev3_motor_set_power(left_motor,  0);
        ev3_motor_set_power(right_motor, 0);
        wait_msec(500);
    #else
    #endif
    sta_cyc(H_CALIB_TASK_CYC);              /* 光センサV値キャリブレーションタスク起動 */
    calib_movement();                       /* キャリブレーション動作 */
    stp_cyc(H_CALIB_TASK_CYC);              /* 光センサV値キャリブレーションタスク停止 */
    color_set_h_calib(h_min, h_max, h_ave);
    printf("H Calibration Finished\n");
    #ifdef ETROBOC_SIM
        wait_msec(500);
    #else   /* RasPike実機 */
        wait_start();                           /* 動作開始待ち（実機：BlueToothボタンで開始） */
    #endif
}

void skip_calibration(){
    color_set_v_calib(V_MIN, V_MAX, V_AVE);
    color_set_s_calib(S_MIN, S_MAX, S_AVE);
    color_set_s_calib(H_MIN, H_MAX, H_AVE);
}

/* 光センサV値キャリブレーションタスク）(10msec周期) */
void v_sensor_calib_task(intptr_t unused) {
    int16_t h,s,v;
    /* 光センサ値取得 */
    color_update();                                                 /* Colorクラス更新 */
    color_get_hsv(&h, &s, &v);                                      /* HSV値取得 */
    /* 最小値と最大値と平均値の更新 */
    if(v < v_min)   v_min = v;
    if(v > v_max)   v_max = v;
    v_ave = (v_min + v_max) / 2;
    /* 表示 */
    color_print_rgbhsv();
    printf("v_calib=, %3d, %3d, %3d, \n", v_min, v_max, v_ave);
    ext_tsk();/* タスク終了 */
}

/* 光センサS値キャリブレーションタスク）(10msec周期) */
void s_sensor_calib_task(intptr_t unused) {
    int16_t h,s,v;
    /* 光センサ値取得 */
    color_update();                                                 /* Colorクラス更新 */
    color_get_hsv(&h, &s, &v);                                      /* HSV値取得 */
    /* 最小値と最大値と平均値の更新 */
    if(s < s_min)   s_min = s;
    if(s > s_max)   s_max = s;
    s_ave = (s_min + s_max) / 2;
    color_print_rgbhsv();
    printf("s_calib=, %3d, %3d, %3d, \n", s_min, s_max, s_ave);
    ext_tsk();/* タスク終了 */
}

/* 光センサS値キャリブレーションタスク）(10msec周期) */
void h_sensor_calib_task(intptr_t unused) {
    int16_t h,s,v;
    /* 光センサ値取得 */
    color_update();                                                 /* Colorクラス更新 */
    color_get_hsv(&h, &s, &v);                                      /* HSV値取得 */
    /* 最小値と最大値と平均値の更新 */
    if(h < h_min)   h_min = h;
    if(h > h_max)   h_max = h;
    h_ave = (h_min + h_max) / 2;
    color_print_rgbhsv();
    printf("h_calib=, %3d, %3d, %3d, \n", h_min, h_max, h_ave);
    ext_tsk();/* タスク終了 */
}

/* キャリブレーション時のモータ制御関数 */
void calib_movement(){

    /* シミュレータ */
    #ifdef ETROBOC_SIM
        /* 右旋回 */
        ev3_motor_set_power(left_motor,  10);
        ev3_motor_set_power(right_motor, -10);
        wait_msec(1500);
        /* 一時停止 */
        ev3_motor_set_power(left_motor,  0);
        ev3_motor_set_power(right_motor, 0);
        wait_msec(500);
        /* 後退 */
        ev3_motor_set_power(left_motor,  -5);
        ev3_motor_set_power(right_motor, -5);
        wait_msec(1500);
        /* 一時停止 */
        ev3_motor_set_power(left_motor,  0);
        ev3_motor_set_power(right_motor, 0);
        wait_msec(500);
        /* 前進 */
        ev3_motor_set_power(left_motor,  5);
        ev3_motor_set_power(right_motor, 5);
        wait_msec(1500);
        /* 一時停止 */
        ev3_motor_set_power(left_motor,  0);
        ev3_motor_set_power(right_motor, 0);
        wait_msec(500);
        /* 左旋回 */
        ev3_motor_set_power(left_motor, -10);
        ev3_motor_set_power(right_motor, 10);
        wait_msec(1500);
        /* 一時停止 */
        ev3_motor_set_power(left_motor,  0);
        ev3_motor_set_power(right_motor, 0);
        wait_msec(500);
        
    /* RasPike実機 */
    #else
        /* 前進 */
        ev3_motor_set_power(left_motor,  50);
        ev3_motor_set_power(right_motor, 50);
        wait_msec(500);
        /* 一時停止 */
        ev3_motor_set_power(left_motor,  0);
        ev3_motor_set_power(right_motor, 0);
        wait_msec(500);
        /* 後退 */
        ev3_motor_set_power(left_motor,  -50);
        ev3_motor_set_power(right_motor, -50);
        wait_msec(500);
        
        /* 一時停止 */
        ev3_motor_set_power(left_motor,  0);
        ev3_motor_set_power(right_motor, 0);          
    #endif
}
