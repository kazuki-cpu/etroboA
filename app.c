#include "app.h"
#include <stdio.h>

/* グローバル変数 */
rgb_raw_t rgb_max;
rgb_raw_t rgb_min;
rgb_raw_t rgb_ave;

/* プロトタイプ宣言 */
void main_task(intptr_t unused);    /* メインタスク(起動時にのみ関数コールされる) */
static void init_global();          /* グローバル変数の初期化 */
static void port_config();          /* ポート初期設定 */
static void wait_start();           /* 動作開始待ち（シミュレータ：自動で開始, 実機：BlueToothボタンで開始） */
static void calibration();          /* キャリブレーション */
static void laptime_tracer();       /* ラップタイム走行 */
void wait_msec(int32_t msec);       /* タスク停止関数（ミリ秒） */

/* メインタスク(起動時にのみ関数コールされる) */
void main_task(intptr_t unused){
    init_global();                  /* グローバル変数の初期化 */
    port_config();                  /* ポート初期設定 */
    wait_start();                   /* 動作開始待ち（シミュレータ：自動で開始, 実機：BlueToothボタンで開始） */
    calibration();                  /* キャリブレーション */
    laptime_tracer();               /* ラップタイム走行 */
                                    /* タブルループNEO */
                                    /* スマートデブリ */
                                    /* スマートキャリー */
                                    /* 停車処理 */
    ext_tsk();                      /* メインタスク終了 */
}

/* グローバル変数の初期化 */
static void init_global(){
    rgb_max.r = 0;
    rgb_min.r = 200;
    rgb_max.g = 0;
    rgb_min.g = 200;
    rgb_max.b = 0;
    rgb_min.b = 200;
}

/* ポート初期設定 */
static void port_config(){
    /* センサー入力ポートの設定 */
    ev3_sensor_config(touch_sensor ,TOUCH_SENSOR);
    ev3_sensor_config(color_sensor ,COLOR_SENSOR);
    ev3_sensor_config(sonar_sensor ,ULTRASONIC_SENSOR);
    ev3_sensor_config(gyro_sensor  ,GYRO_SENSOR);
    /* モーター出力ポートの設定 */
    ev3_motor_config(arm_motor     ,LARGE_MOTOR);
    ev3_motor_config(left_motor    ,MEDIUM_MOTOR);
    ev3_motor_config(right_motor   ,MEDIUM_MOTOR);
}

/* 動作開始待ち（シミュレータ：自動で開始, 実機：BlueToothボタンで開始） */
static void wait_start(){
    while(true){
        bool_t pressed_flg;
        pressed_flg = ev3_touch_sensor_is_pressed(touch_sensor);
        if(pressed_flg){
            printf("[Main Task] Simulator is ready\n");
            break;
        }
        wait_msec(50);
    }
}

/* キャリブレーション */
static void calibration(){
    printf("[Main Task] Calibration Start\n");
    /* キャリブレーションタスク起動 */
    sta_cyc(CALIBRATION_TASK_CYC);
    /* 右旋回 */
    ev3_motor_set_power(left_motor,  10);
    ev3_motor_set_power(right_motor, -10);
    wait_msec(1500);
    /* 一時停止 */
    ev3_motor_set_power(left_motor,  0);
    ev3_motor_set_power(right_motor, 0);
    wait_msec(1000);
    /* 前進 */
    ev3_motor_set_power(left_motor,  5);
    ev3_motor_set_power(right_motor, 5);
    wait_msec(1000);
    /* 後退 */
    ev3_motor_set_power(left_motor,  -5);
    ev3_motor_set_power(right_motor, -5);
    wait_msec(4000);
    /* 前進 */
    ev3_motor_set_power(left_motor,  5);
    ev3_motor_set_power(right_motor, 5);
    wait_msec(2500);
    /* 左旋回 */
    ev3_motor_set_power(left_motor, -10);
    ev3_motor_set_power(right_motor, 10);
    wait_msec(1500);
    /* 一時停止 */
    ev3_motor_set_power(left_motor,  0);
    ev3_motor_set_power(right_motor, 0);
    /* キャリブレーションタスク停止 */
    stp_cyc(CALIBRATION_TASK_CYC);
    /* キャリブレーション終了 */
    printf("[Main Task] Calibration Finished\n");
    printf("[Main Task] min:%d, max:%d, ave:%d\n",rgb_min.g, rgb_max.g, rgb_ave.g);
    wait_msec(1000);
        
    /* （デモ用）逆走にする */
    ev3_motor_set_power(left_motor, -10);
    ev3_motor_set_power(right_motor, 10);
    wait_msec(3200);
    ev3_motor_set_power(left_motor,  0);
    ev3_motor_set_power(right_motor, 0);
    wait_msec(1000);
}

/* ラップタイム走行 */
static void laptime_tracer(){
    sta_cyc(LINE_TRACER_TASK_CYC);  /* ライントレースタスクの起動 */
}

/* タスク停止関数（ミリ秒） */
void wait_msec(int32_t msec){
    dly_tsk(msec * 1000 / 0.6);
}
