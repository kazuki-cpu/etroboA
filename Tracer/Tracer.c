#pragma once
#include "app.h"
#include "Color.h"
#include "Odometry.h"
#include "Tracer.h"

typedef enum {
    WHITE,
    BLACK,
    EDGE,
    MIDDLE,
    BLUE,
    CENTER
} DETECT_TARGET;
static DETECT_TARGET target = WHITE;

int bias;
extern float angle_diff;

void tracer_task(intptr_t unused) {
    color_update();
    
    /* 計測器初期化 */

    if(fabsf(angle_diff) < 5){
            bias = 0;
        }
        else{
            if(angle_diff>=0){
                bias = -5;
            }
            else{
                bias = 5;
            }
        }
    
    switch(target) {        
        case WHITE:
            int16_t v = color_get_v();
            int16_t v_ave = color_get_v_ave();
            if(v > v_ave) {
                target = BLACK;
            }
            else{
                ev3_motor_set_power(left_motor, -53 - bias);
                ev3_motor_set_power(right_motor, 53 + bias);
            }
            break;
        case BLACK:
            if(v < v_ave) {;
                target = EDGE;
                sta_cyc(ODOMETRY_TASK_CYC);
            }
            else{
                ev3_motor_set_power(left_motor, 53 + bias);
                ev3_motor_set_power(right_motor, -53 - bias);
            }
            break;
        case EDGE:
            if(v > v_ave) {
                target = MIDDLE;
                ev3_motor_stop(left_motor, true);
                ev3_motor_stop(right_motor, true);
                stp_cyc(ODOMETRY_TASK_CYC);
                float detected_dir = Direction_calc();
                odom_Direction_setDirection(detected_dir);
                sta_cyc(ODOMETRY_TASK_CYC);
            }
            else{
                ev3_motor_set_power(left_motor, 45 + bias);
                ev3_motor_set_power(right_motor, 45 - bias);
            }
            break;
        case MIDDLE:                             /* 青線走行状態 */
            if(odom_Direction_getDirection() < 90.0) {
                ev3_motor_set_power(left_motor, -53 - bias);
                ev3_motor_set_power(right_motor, 53 + bias);
            } else {
                ev3_motor_set_power(left_motor, 53 + bias);
                ev3_motor_set_power(right_motor, -53 - bias);
            }
            // 指定方位の一定範囲内に収まったら,移動開始
            if( (odom_Direction_getDirection() > (89.0)) && (odom_Direction_getDirection() < (91.0)) ) {;
                //motorをストップ
                ev3_motor_stop(left_motor, true);
                ev3_motor_stop(right_motor, true);
                //一旦オドメトリタスクをストップ&待ち
                stp_cyc(ODOMETRY_TASK_CYC);
                wait_msec(50);
                last_dir = cur_dir;
                printf("last_dir = cur_dir = %lf\n", cur_dir);
                
                wait_msec(50);
                state = MOVE;
                //最後の方位を代入＆オドメトリタスク再開
                odom_Direction_setDirection(last_dir);
                sta_cyc(ODOMETRY_TASK_CYC);
                                                                               
                printf("state = MOVE\n");
            }
            break;
        case BLUE:                            /* 黒線走行状態 */
            if(s > S_AVERAGE && v > V_DARK){       /* 青線を検知したら */
                target = CENTER;             /* 次の走行状態に遷移 */
            }
            else{
                white_count++;
            }
            break;
        case CENTER:
               if(white_count>10){
                   bias_grid.gridX=;
                   bias_grid.gridY=;
                   diretion =
               }
               else if(){

               }        
            break;
        default:
            break;
    }
    printf("scene=%s, ",scene_param[*scene_state].current_scene);     /* シーン状態の表示 */
    printf("edge=%s, \n",edge_table[*edge_state].current_edge);           /* エッジ状態の表示 */
}

float incideDirection_calc(){
    return arccos(20.0/odom_Distance_getDistance());
}
