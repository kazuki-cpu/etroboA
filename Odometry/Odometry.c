#include "app.h"
#include "Odometry.h"

float distance;
float distanceR;
float distanceL;
float pre_angleL;
float pre_angleR;
float distance;
float direction; //現在の方位

/* 初期化関数 */
void odom_Distance_reset(){
    //各変数の値の初期化
    distance = 0.0;
    distanceR = 0.0;
    distanceL = 0.0;
    //モータ角度の過去値に現在値を代入
    pre_angleL = ev3_motor_get_counts(left_motor);
    pre_angleR = ev3_motor_get_counts(right_motor);
}

/* 距離更新(10ms間の移動距離を毎回加算している) */
void odom_Distance_update(){
    float cur_angleL = ev3_motor_get_counts(left_motor); //左モータ回転角度の現在値
    float cur_angleR = ev3_motor_get_counts(right_motor);//右モータ回転角度の現在値

    // 10ms間の走行距離 = ((円周率 * タイヤの直径) / 360) * (モータ角度過去値 - モータ角度現在値)
    distanceL = ((PI * TIRE_DIAMETER) / 360.0) * (cur_angleL - pre_angleL);  // 左モータ距離
    distanceR = ((PI * TIRE_DIAMETER) / 360.0) * (cur_angleR - pre_angleR);  // 右モータ距離
    distance += (distanceL + distanceR) / 2.0; //左右タイヤの走行距離を足して割る

    //モータの回転角度の過去値を更新
    pre_angleL = cur_angleL;
    pre_angleR = cur_angleR;
}

void odom_print_distance(){
    printf("dist=%.1lf, ",distance);
}

/* 走行距離を取得 */
float odom_Distance_getDistance(){
    return distance;
}

/* 右タイヤの10ms間の距離を取得 */
float odom_Distance_getDistanceRight(){
    return distanceR;
}

/* 左タイヤの10ms間の距離を取得 */
float odom_Distance_getDistanceLeft(){
    return distanceL;
}

/* 方位リセット */
void odom_Direction_reset(){
    direction = 0.0;
}

 /* 方位を取得(右旋回が正転) */
float odom_Direction_getDirection(){
    return direction;
}

/* 方位を更新 */
void odom_Direction_update(){
    //(360 / (2 * 円周率 * 車体トレッド幅)) * (右進行距離 - 左進行距離)
    direction += (360.0 / (2.0 * PI * TREAD)) * (odom_Distance_getDistanceLeft() - odom_Distance_getDistanceRight());
}
