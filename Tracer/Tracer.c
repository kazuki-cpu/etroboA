#pragma once
#include "app.h"
#include "Odometry.h"
#include "Tracer.h"


typedef enum {
    AHEAD,
    TURN,
    END
} RUN_STATE;

int bias;
extern float angle_diff;

void tracer_task(intptr_t unused) {
    //static RUN_STATE state = AHEAD;
    static RUN_STATE state = TURN;

    /* 計測器初期化 */
    //odom_Distance_reset();
    //odom_Direction_reset();
    if(5 > abs(angle_diff)){
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
    
    switch(state) {        
        case AHEAD:
            //左右車輪駆動
            ev3_motor_set_power(left_motor, 45 + bias);//53
            ev3_motor_set_power(right_motor, 45 - bias);
            //1000mm以上前進したら，次状態遷移
            if(odom_Distance_getDistance() > 1000.0) {
                state = END;
            }
            break;
        case TURN:
            ev3_motor_set_power(left_motor, -69 - bias);//64,61,58,55
            ev3_motor_set_power(right_motor, 55 + bias);//40,37,34,31
            if(odom_Direction_getDirection() > 90.0) {;
                state = END;
            }
            break;
        case END:
            ev3_motor_stop(left_motor, true);
            ev3_motor_stop(right_motor, true);
            stp_cyc(ODOMETRY_TASK_CYC);
            wait_msec(1000);
            sta_cyc(ODOMETRY_TASK_CYC);
            break;
        default:
            break;
    }
}

/* シーン状態とエッジ状態の遷移関数 */
static void white_count(enum SCENE_STATE *scene_state, enum EDGE_STATE *edge_state, int16_t s, int16_t v){
    switch(*edge_state){
        case EDGE_BLUE:                             /* 青線走行状態 */
            if(s < S_COLORFUL && v > V_DARK){       /* 白線を検知したら */
                *edge_state = EDGE_WHITE;               /* 次の走行状態に遷移 */
            }
            break;
        case EDGE_WHITE:                            /* 黒線走行状態 */
            if(s > S_AVERAGE && v > V_DARK){       /* 青線を検知したら */
                *edge_state = END;             /* 次の走行状態に遷移 */
            }
            else{
                white_count++;
            }
        case END:
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
