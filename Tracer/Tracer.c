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
    if(5 > fabsf(angle_diff)){
            bias = 0;
            printf("angle_diff=%lf, bias=%lf\n", angle_diff, bias);
    }
    else{
        if(angle_diff>=0){
                bias = -5;
        }
        else{
            bias = 5;
        }
    }
    //printf("angle_diff=%lf, bias=%lf\n", angle_diff, bias);
    
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
            ev3_motor_set_power(left_motor, -50 - bias);//64,61,58,55
            ev3_motor_set_power(right_motor, 50 + bias);//40,37,34,31
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

