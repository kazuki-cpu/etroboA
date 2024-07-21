#ifdef __cplusplus
extern "C" {
#endif

#include "ev3api.h"

/* タスク優先度 */
#define MAIN_PRIORITY    (TMIN_APP_TPRI + 1) /* メインタスク */
#define TRACER_PRIORITY  (TMIN_APP_TPRI + 2) /* ライントレースタスク */
#define CALIBRATION_PRIORITY  (TMIN_APP_TPRI + 2) /* キャリブレーションタスク */

/* タスク周期の定義 */
#define LINE_TRACER_PERIOD  (100 * 1000 / 0.6 ) /* ライントレースタスク:100msec周期 */
#define CALIBRATION_PERIOD  (100 * 1000 / 0.6) /* キャリブレーションタスク:100msec周期 */

/* グローバル変数 */
extern rgb_raw_t rgb_max;
extern rgb_raw_t rgb_min;
extern rgb_raw_t rgb_ave;

/* センサーポートの定義 */
static const sensor_port_t
    touch_sensor    = EV3_PORT_1,
    color_sensor    = EV3_PORT_2,
    sonar_sensor    = EV3_PORT_3,
    gyro_sensor     = EV3_PORT_4;

/* モーターポートの定義 */
static const motor_port_t
    arm_motor       = EV3_PORT_A,
    left_motor      = EV3_PORT_C,
    right_motor     = EV3_PORT_B;

/* STACK_SIZE */
#ifndef STACK_SIZE
#define STACK_SIZE      (4096)
#endif

#ifndef TOPPERS_MACRO_ONLY
/* プロトタイプ宣言 */
extern void main_task(intptr_t exinf);          /* メインタスク(起動時にのみ関数コールされる) */
extern void tracer_task(intptr_t exinf);        /* ライントレースタスク(100msec周期） */
extern void calibration_task(intptr_t exinf);   /* キャリブレーションタスク(100msec周期) */
extern void wait_msec(int32_t msec);                   /* タスク停止関数（ミリ秒） */
#endif

#ifdef __cplusplus
}
#endif
