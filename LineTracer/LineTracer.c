#include "app.h"
#include "LineTracer.h"
#include <stdio.h>

/* 関数プロトタイプ宣言 */
static int16_t get_rgb_diff();                                /* 反射光とキャリブレーション値との差を取得*/
static int calc_rotation(int16_t green_diff);                   /* 旋回量を計算 */
static void motor_control(int rotation);                        /* 走行モータ制御 */
static void display_position(int16_t green_diff);               /* 車体位置のグラフィカル表示 */

/* ライントレースタスク(100msec周期） */
void tracer_task(intptr_t unused) {
    int16_t green_diff;                                         /* RGB値差分 */
    int rotation;                                               /* 旋回量 */
    green_diff = get_rgb_diff();                                  /* RGB値の現在値とキャリブレーション値との差を取得 */
    rotation = (int)calc_rotation(green_diff);                         /* 旋回量を計算 */
    motor_control(rotation);                                    /* 走行モータ制御 */
    display_position(green_diff);                                 /* 車体位置のグラフィカル表示 */
    ext_tsk();                                                  /* タスク終了 */                                          
}

/* 反射光とキャリブレーション値との差を取得*/
static int16_t get_rgb_diff(){
    rgb_raw_t rgb_raw;
    int16_t green_diff;
    ev3_color_sensor_get_rgb_raw(color_sensor, &rgb_raw);       /* カラーセンサ値の取得 */
    green_diff = rgb_ave.g - rgb_raw.g;                         /* キャリブレーション値との差分を計算（green） */
    return green_diff;
}

/* 旋回量を計算 */
static int calc_rotation(int16_t green_diff){
    int rotation;
    rotation = (int)(green_diff * ROTATE_COEF);                 /* ステアリング操舵量を計算（Greenで計算） */
    return rotation;
}

/* 走行モータ制御 */
static void motor_control(int rotation){
    int power_L;                                                /* 左モータパワー */
    int power_R;                                                /* 右モータパワー */
    power_L = (int)(BASE_SPEED + (rotation * LEFT_EDGE));       /* 左モータパワー計算 */
    power_R = (int)(BASE_SPEED - (rotation * LEFT_EDGE));       /* 右モータパワー計算 */
    ev3_motor_set_power(left_motor, power_L);                   /* 左モータパワー設定 */
    ev3_motor_set_power(right_motor, power_R);                  /* 右モータパワー設定 */
}

/* 車体位置のグラフィカル表示 */
static void display_position(int16_t green_diff){
    int range;
    int position;
    range = rgb_max.g - rgb_min.g;
    position = (10*green_diff + range/2) / range;
    switch(position){
        case -10: printf("#---------|---------- diff=%5d\n",green_diff);break;
        case -9 : printf("-#--------|---------- diff=%5d\n",green_diff);break;
        case -8 : printf("--#-------|---------- diff=%5d\n",green_diff);break;
        case -7 : printf("---#------|---------- diff=%5d\n",green_diff);break;
        case -6 : printf("----#-----|---------- diff=%5d\n",green_diff);break;
        case -5 : printf("-----#----|---------- diff=%5d\n",green_diff);break;
        case -4 : printf("------#---|---------- diff=%5d\n",green_diff);break;
        case -3 : printf("-------#--|---------- diff=%5d\n",green_diff);break;
        case -2 : printf("--------#-|---------- diff=%5d\n",green_diff);break;
        case -1 : printf("---------#|---------- diff=%5d\n",green_diff);break;
        case  0 : printf("----------#---------- diff=%5d\n",green_diff);break;
        case  1 : printf("----------|#--------- diff=%5d\n",green_diff);break;
        case  2 : printf("----------|-#-------- diff=%5d\n",green_diff);break;
        case  3 : printf("----------|--#------- diff=%5d\n",green_diff);break;
        case  4 : printf("----------|---#------ diff=%5d\n",green_diff);break;
        case  5 : printf("----------|----#----- diff=%5d\n",green_diff);break;
        case  6 : printf("----------|-----#---- diff=%5d\n",green_diff);break;
        case  7 : printf("----------|------#--- diff=%5d\n",green_diff);break;
        case  8 : printf("----------|-------#-- diff=%5d\n",green_diff);break;
        case  9 : printf("----------|--------#- diff=%5d\n",green_diff);break;
        case  10: printf("----------|---------# diff=%5d\n",green_diff);break;
        default : printf("##########|########## diff=%5d\n",green_diff);break;
    }
}
