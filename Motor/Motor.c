#include "app.h"
#include "LineTracer_conf.h"

int edge = 1;
int rot_max = 50;
int base_speed;
int rotation;

void motor_set_edge(int edge_ref){
    edge = edge_ref;
}

void motor_set_range(int rot_max_ref){
    rot_max = rot_max_ref;
}

void motor_set_power(int base_speed_ref, int rotation_ref){
    /* メンバ変数にセット */
    base_speed = base_speed_ref;
    rotation = rotation_ref;
    /* 旋回量上限 */
    if(rotation < -rot_max) rotation = -rot_max; 
    if(rotation > rot_max) rotation = rot_max; 
    /* モータパワーのセット */
    ev3_motor_set_power(left_motor,  base_speed + rotation * edge);
    ev3_motor_set_power(right_motor, base_speed - rotation * edge);
}

void motor_set_power_inout(enum TRACE_MODE trace_mode, int edge){
    switch(trace_mode){
        case EDGE_IN:
            if(edge==1){//leftEDGE
                ev3_motor_set_power(left_motor, 53);
                ev3_motor_set_power(right_motor, 0);
            }
            else if(edge=-1){//rightEDGE
                ev3_motor_set_power(left_motor, 0);
                ev3_motor_set_power(right_motor, 53);
            }
            else{
            }
            break;
        case EDGE_OUT:
            if(edge==1){
                ev3_motor_set_power(left_motor, 0);
                ev3_motor_set_power(right_motor, 53);
            }
            else if(edge=-1){
                ev3_motor_set_power(left_motor, 53);
                ev3_motor_set_power(right_motor, 0);
            }
            else{
            }
            break;
    }
}

void motor_print_fwd(){
    printf("fwd:%3d, ",base_speed);
}

void motor_print_rot(){
    printf("rot:%3d, ",rotation);
}
