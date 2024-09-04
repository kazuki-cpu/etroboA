#include "app.h"
#include "LineTracer.h"
#include "LineTracer_conf.h"
#include "Odometry.h"
#include "Color.h"
#include "Pid.h"
#include "Motor.h"
#include <stdio.h>

/***************************************************************************************************
    tracer周期処理（10ms）
 ***************************************************************************************************/
bool_t tracer_10ms_cycle(){
    static enum SCENE_STATE scene_state = SCENE_LAP_1_STRAIGHT;     /* シーン状態 */
    bool_t tracer_end_flg = false;

    tracer_scene_transition(&scene_state);      /* シーン判定＆遷移 */
    tracer_steering(scene_state);               /* 旋回量計算＆モーター制御 */
    tracer_print_info(scene_state);             /* 情報表示 */

    /* シーン状態が末尾まで来たらtracer終了 */
    if(scene_state == SCENE_WLOOP_52_END){
        tracer_end_flg = true;
    }
    
    return tracer_end_flg;
}

/***************************************************************************************************
    tracer関数定義（Public）
 ***************************************************************************************************/

void tracer_init(bool_t flg){
    if(flg){
        #ifdef ETROBOC_SIM  /* シミュレータ */
            /* 左旋回 */
            ev3_motor_set_power(left_motor,  -10);
            ev3_motor_set_power(right_motor, 10);
            wait_msec(200);
            /* 前進 */
            ev3_motor_set_power(left_motor,  20);
            ev3_motor_set_power(right_motor, 20);
            wait_msec(400);
            /* 右旋回 */
            ev3_motor_set_power(left_motor,  10);
            ev3_motor_set_power(right_motor, -10);
            wait_msec(200);
            /* 右旋回 */
            ev3_motor_set_power(left_motor,  0);
            ev3_motor_set_power(right_motor, 0);
            wait_msec(100);
        #else
        #endif
        odom_Distance_reset();                                          /* オドメトリリセット */
    }
}

/***************************************************************************************************
    tracer関数定義（Private）
 ***************************************************************************************************/

/* シーン状態の遷移関数 */
void tracer_scene_transition(enum SCENE_STATE *scene_state){

    /* フラグとカウンタの定義 */
    bool_t next_scene_flg = false;
    bool_t marker_flg = false;
    static int marker_count = 0;

    /* パラメータの取得 */
    float odom_target               = scene_param[*scene_state].odom_target;
    enum TRACE_MODE trace_mode      = scene_param[*scene_state].trace_mode;

    /* オドメトリ走行距離によるシーン遷移判定*/
    float odom_distance;                                            /* オドメトリ累積移動距離 */
    odom_distance = odom_Distance_getDistance();                      /* 情報取得（累積移動距離） */
    if(odom_target != NA){
        if(odom_distance >= odom_target){
            next_scene_flg = true;
        }
    }
    /* 光センサ色によるシーン遷移判定 */
    else{
        switch(trace_mode){
            case EDGE_BLACK:    if(color_marker_is_detected(S_THRE_MID))        marker_flg     = true;  break;  /* 青線検知 */
            case EDGE_BLUE:     if(!color_marker_is_detected(S_THRE_MID))       marker_flg     = true;  break;  /* 黒線検知 */
            case EDGE_IN:       if(!color_brighter_than_average(V_THRE_LOW))    next_scene_flg = true;  break;  /* 黒線中央検知*/
            case EDGE_OUT:      if(color_brighter_than_average(V_THRE_AVE))    next_scene_flg = true;  break;  /* 反対側エッジ検知 */
            default:break;
        }
    }
    /* マーカーを一定回数カウントでシーン遷移 */
    if(marker_flg){
        marker_count++;
        if(marker_count >= MARKER_COUNT){
            next_scene_flg = true;     /* シーン遷移 */
        }
    }else{
        marker_count = 0;
    }
    /* 条件を満たした場合シーン遷移 */
    if(next_scene_flg){
        *scene_state = *scene_state + 1;    /* シーン遷移 */
        marker_count = 0;            /* カウントリセット */
        odom_Distance_reset();              /* オドメトリ値リセット */
        pid_reset();                        /* PIDリセットフラグセット*/
    }
}

/* 旋回関数 */
void tracer_steering(enum SCENE_STATE scene_state){
    /* パラメータの取得 */
    int base_speed              = scene_param[scene_state].base_speed;
    int rot_max                 = scene_param[scene_state].rot_max;
    enum TRACE_MODE trace_mode  = scene_param[scene_state].trace_mode;
    float kp                    = scene_param[scene_state].kp;
    float ki                    = scene_param[scene_state].ki;
    float kd                    = scene_param[scene_state].kd;
    int edge                    = scene_param[scene_state].edge;
    int rot_const               = scene_param[scene_state].rot_const;
    int curve_offset            = scene_param[scene_state].curve_offset;

    /* 旋回量の計算 */
    int rotation = 0;
    switch(trace_mode){
        case EDGE_BLACK:
            rotation = get_pid_v(kp, ki, kd);
            /* 青線検知している場合は固定旋回量 */
            if(color_marker_is_detected(S_THRE_LOW)){  
                rotation = curve_offset;
            }
            break;
        case EDGE_BLUE:
            rotation = get_pid_s(kp, ki, kd);                      
            /* 黒線検知している場合は固定旋回量 */                  
            if(!color_marker_is_detected(S_THRE_HIGH)){
                rotation = curve_offset;
            }
            break;
        case EDGE_IN:
            //ev3_motor_stop(left_motor, false);
            //ev3_motor_stop(right_motor, false);
            //wait_msec(10000);
        case EDGE_OUT:
            rotation = -1 * rot_const;
            break;
        default:
            break;
    }

    /* モーター制御 */
    motor_set_edge(edge);
    motor_set_range(rot_max);
    
    if(trace_mode==EDGE_IN||EDGE_OUT){
        motor_set_power(trace_mode, edge);
    }
    else{
    motor_set_power(base_speed, rotation);
    }

}

void tracer_print_info(enum SCENE_STATE scene_state){
    motor_print_fwd();                                              /* 表示 */
    motor_print_rot();                                              /* 表示 */
    odom_print_distance();                                          /* 表示 */
    color_print_rgbhsv();                                           /* 表示 */
    printf("scene=%s, ",scene_param[scene_state].current_scene);    /* 表示 */
    printf("\n");                                                   /* 表示 */
}
