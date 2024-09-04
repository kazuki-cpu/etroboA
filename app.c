#include <stdio.h>
#include "app.h"
#include "Calibration.h"
#include "Manager.h"

/***************************************************************************************************
    タスク定義
 ***************************************************************************************************/

/* メインタスク(起動時にのみ関数コールされる) */
void main_task(intptr_t unused){
    port_config();          /* ポート初期設定 */
    wait_start();           /* 動作開始待ち（シミュレータ：自動で開始, 実機：BlueToothボタンで開始） */
    wait_msec(1000);
    calibration(false);     /* キャリブレーション（true:実行、false:スキップ） */
    wait_msec(1000);
    start_manager(TRACER);   /* 競技開始（開始する競技を指定：TRACER,DEBRI） */
    ext_tsk();              /* メインタスク終了 */
}

/***************************************************************************************************
    メインタスク用の関数
 ***************************************************************************************************/

/* ポート初期設定 */
void port_config(){
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

/* 動作開始待ち（シミュレータ：自動またはスペースキーで開始, 実機：BlueToothボタンで開始） */
void wait_start(){
    while(true){
        bool_t pressed_flg;
        pressed_flg = ev3_touch_sensor_is_pressed(touch_sensor);
        if(pressed_flg){
            printf("Start Botton Pressed\n");
            break;
        }
        wait_msec(50);
        printf("*");
    }
}

/***************************************************************************************************
    共通関数
 ***************************************************************************************************/

/* タスク停止関数（ミリ秒） */
void wait_msec(int32_t msec){
    #ifdef ETROBOC_SIM
        dly_tsk(msec * 1000 / 0.6);
    #else
        dly_tsk(msec * 1000);
    #endif
}
