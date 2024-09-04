
#include "app.h"
#include "Color.h"
#include "Pid.h"

bool_t reset_flg = false;

/***************************************************************************************************
    関数定義
 ***************************************************************************************************/

void pid_reset(){
    reset_flg = true;
}

/* 明度VによるPID制御値取得関数 */
int get_pid_v(float kp, float ki, float kd){
    static int16_t v_prev;
    int16_t v = color_get_v();
    int16_t v_ave = color_get_v_ave();
    float p;
    static float i = 0;
    float d;
    if(reset_flg){
        p = (float)(v_ave - v);        /* 比例制御P */
        i = 0.0;
        d = 0.0;
        reset_flg = false;
    }else{
        p = (float)(v_ave - v);            /* 比例制御P */
        i += p * 0.01f;             /* 積分制御I */
        d = (v_prev - v)/0.01f;     /* 比例制御D */
        if(i > 10000) printf("too much integral value\n");
    }
    v_prev = v;
    return (int)(kp * p + ki * i + kd * d);
}

/* 彩度SによるPID制御値取得関数 */
int get_pid_s(float kp, float ki, float kd){
    static int16_t s_prev;
    int16_t s = color_get_s();
    int16_t s_ave = color_get_s_ave();
    float p;
    static float i = 0;
    float d;
    if(reset_flg){
        p = (float)(s_ave * S_AVE_OFFSET - s);            /* 比例制御P */
        i = 0.0;
        d = 0.0;
        reset_flg = false;
    }else{
        p = (float)(s_ave * S_AVE_OFFSET - s);            /* 比例制御P */
        i += p * 0.01f;             /* 積分制御I */
        d = (s_prev - s)/0.01f;     /* 比例制御D */
        if(i > 10000) printf("too much integral value\n");
    }
    s_prev = s;
    return -1 * (int)(kp * p + ki * i + kd * d);
}
