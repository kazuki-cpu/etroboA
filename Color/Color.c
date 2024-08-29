#include <stdio.h>
#include "app.h"
#include "Color.h"

rgb_raw_t rgb_raw;  /* RGB値 */
rgb_raw_t rgb_adjusted;  /* RGB値 */
int16_t h, s ,v;    /* 色相H *//* 彩度S *//* 明度V */

int16_t s_min;          /* 彩度Sの平均値（目標値） */
int16_t s_max;          /* 彩度Sの平均値（目標値） */
int16_t s_ave;          /* 彩度Sの平均値（目標値） */
int16_t v_min;          /* 明度Vの平均値（目標値） */
int16_t v_max;          /* 明度Vの平均値（目標値） */
int16_t v_ave;          /* 明度Vの平均値（目標値） */

/***************************************************************************************************
    Public
 ***************************************************************************************************/
/* センサ取得値更新関数 */
void color_update(){
    ev3_color_sensor_get_rgb_raw(color_sensor, &rgb_raw);   /* RGB値を取得 */
    adjust_rgb();                                           /* RGB値を補正 */
    rgb2hsv();                                              /* RGB値からHSV値に変換 */
}

/* HSV値取得関数 */
void color_get_hsv(int16_t *h_ref, int16_t *s_ref, int16_t *v_ref){
    *h_ref = h;
    *s_ref = s;
    *v_ref = v;
}

/* キャリブレーション値セット */
void color_set_v_calib(int16_t v_min_ref, int16_t v_max_ref, int16_t v_ave_ref){
    v_min = v_min_ref;
    v_max = v_max_ref;
    v_ave = v_ave_ref;
}

/* キャリブレーション値セット */
void color_set_s_calib(int16_t s_min_ref, int16_t s_max_ref, int16_t s_ave_ref){
    s_min = s_min_ref;
    s_max = s_max_ref;
    s_ave = s_ave_ref;
}

/* 色線（マーカー）検知 */
bool_t color_marker_is_detected(float s_threshold){
    if((float)s > s_ave * s_threshold && (float)v > v_ave * V_THRE_DARK){ return true; }else{ return false;}
}

/* 明度Vの判定 */
bool_t color_brighter_than_average(float v_threshold){
    if((float)v > v_ave * v_threshold){ return true; }else{ return false; }
}

/* RGB値とHSV値の表示 */
void color_print_rgbhsv(){
    printf("rgbhsv=, %3d, %3d, %3d, %3d, %3d, %3d, ",rgb_adjusted.r, rgb_adjusted.g, rgb_adjusted.b,h,s,v);
}

int16_t color_get_v_ave(){
    return v_ave;
}

int16_t color_get_s_ave(){
    return s_ave;
}

int16_t color_get_v(){
    return v;
}

int16_t color_get_s(){
    return s;
}

/***************************************************************************************************
    Private
 ***************************************************************************************************/

/* RGB値補正関数 */
void adjust_rgb(){
    /* RGB値補正（シミュレーション向けの補正値） */
    rgb_adjusted.r = rgb_raw.r * RGB_GAIN_RED;
    rgb_adjusted.g = rgb_raw.g * RGB_GAIN_GREEN;
    rgb_adjusted.b = rgb_raw.b * RGB_GAIN_BLUE;
}

/* RGB値からHSV値に変換する関数 */
void rgb2hsv(){
    int16_t max,min;
    /* RGB最大値計算 */
    max = rgb_adjusted.r;                
    if (rgb_adjusted.g > max) max = rgb_adjusted.g;
    if (rgb_adjusted.b > max) max = rgb_adjusted.b;
    /* RGB最小値計算 */
    min = rgb_adjusted.r;
    if (rgb_adjusted.g < min) min = rgb_adjusted.g;
    if (rgb_adjusted.b < min) min = rgb_adjusted.b;
    /* 色相H計算 */
    if(rgb_adjusted.r == max) h = 60 * (rgb_adjusted.g - rgb_adjusted.b) / (max - min);
    if(rgb_adjusted.g == max) h = 60 * (rgb_adjusted.b - rgb_adjusted.r) / (max - min) + 120;
    if(rgb_adjusted.b == max) h = 60 * (rgb_adjusted.r - rgb_adjusted.g) / (max - min) + 240;
    /* 彩度S計算 */
    s = 255 * (max - min) / max;
    /* 明度V計算 */
    v = max;
}
