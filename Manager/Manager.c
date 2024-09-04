#include "app.h"
#include "LineTracer.h"
//#include "Debri.h"
#include "Manager.h"
#include "Odometry.h"
#include "Color.h"

enum MANAGE_STATE manage_state;

void start_manager(enum MANAGE_STATE manage_state_ref){
    manage_state = manage_state_ref;
    switch(manage_state){
        case TRACER:    tracer_init(true);  break;
        //case DEBRI:     debri_init(true);   break;
        case CARRY:     break;
        case PARKING:   break;
        default:break;
    }
    sta_cyc(MANAGER_TASK_CYC);  /* タスクの起動 */
}

/***************************************************************************************************
    タスク定義
 ***************************************************************************************************/
/* マネージャータスク(10msec周期） */
void manager_task(intptr_t unused) {
    bool_t transition_flg = false;

    odom_Distance_update();                                         /* 情報更新（Odometry) */
    color_update();                                                 /* 情報更新（Color） */

    switch(manage_state){
        case TRACER:    transition_flg = tracer_10ms_cycle();   break;
        //case DEBRI:     transition_flg = debri();    break;
        case CARRY:     break;
        case PARKING:   break;
        default:break;
    }
    if(transition_flg){
        manage_state = manage_state + 1;
    }

    check_stop_botton();                                            /* ボタン押下で動作停止 */
    ext_tsk();                                                      /* タスク終了 */ 
}

/* 動作停止（シミュレータ：スペースキー, 実機：BlueToothボタン） */
void check_stop_botton(){
    if(ev3_touch_sensor_is_pressed(touch_sensor)){
        printf("Stop Botton Pressed\n");
        ev3_motor_stop(left_motor, false);
        ev3_motor_stop(right_motor, false);
        stp_cyc(MANAGER_TASK_CYC);
        wait_msec(1000);
    }
}
