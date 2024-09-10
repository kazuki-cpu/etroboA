#pragma once

#include "ev3api.h"
#include "app.h"
#include "Odometry.h"
#include "Grid.h"

#if defined(BUILD_MODULE)
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif

#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif


// 構造体に格納されている目標座標の数
#define GRID_NUM 6
#define GRID_SIZE 352.0 //座標のマス幅（352mm）
#define GUADRANT 1 //Rコースは=1,Lコースは-1

/**
 * Global variables
 */
typedef enum {
    TURN, // 目標座標の方位へ向くまで旋回
    MOVE, // 目標座標に到達するまで前進
    END   // 構造体に格納されている座標を移動しきったら停止
} RUN_STATE;
static RUN_STATE state = TURN;

//目標座標情報の構造体
struct GRID_XY {
    int gridX;
    int gridY;
};
 struct GRID_XY target_grid[GRID_NUM] = {{0,1},
                                            {1,1},
                                            {2,1},
                                            {2,2},
                                            {2,3},
                                            {3,3} };

static float grid_distance = 0.0; //現在座標から目標座標までの距離
static float grid_distanceX = 0.0;
static float grid_distanceY = 0.0;
static float grid_direction = 0.0;//現在座標から目標座標の方位

int edge = 1;
int bias;
extern float angle_diff;
extern float direction; //現在の方位

void grid_task(intptr_t unused) {

        // 計測器の現在値を格納
        cur_dis = odom_Distance_getDistance();
        cur_dir = odom_Direction_getDirection();
        float last_dir;

        /*
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
        printf("angle_diff=%lf, bias=%d\n", angle_diff, bias);
        */
        Grid_setBias();//↑/**/関数化

        switch(state) {
        case TURN:
            // 指定方位まで旋回する
            if(cur_dir < target_dir) {
                //ev3_motor_set_power(left_motor, -53 - bias);
                //ev3_motor_set_power(right_motor, 53 + bias);
                Grid_steerTurn_P();
            } else {
                //ev3_motor_set_power(left_motor, 53 + bias);
                //ev3_motor_set_power(right_motor, -53 - bias);
                Grid_steerTurn_N();
            }
            // 指定方位の一定範囲内に収まったら,移動開始
            if( (cur_dir > (target_dir-0.5)) && (cur_dir < (target_dir+0.5)) ) {;
                last_dir = cur_dir;
                                                                               
                //motorをストップ
                //ev3_motor_stop(left_motor, true);
                //ev3_motor_stop(right_motor, true);
                Grid_steerStop();                                                                
                /*
                //一旦オドメトリタスクをストップ&待ち
                stp_cyc(ODOMETRY_TASK_CYC);
                wait_msec(50);
                //last_dir = cur_dir;
                //printf("last_dir = cur_dir = %lf\n", cur_dir);
                odom_Direction_reset();
                wait_msec(50);
                //state = MOVE;
                //最後の方位を代入＆オドメトリタスク再開
                odom_Direction_setDirection(last_dir);
                sta_cyc(ODOMETRY_TASK_CYC);
                */
                odom_Direction_resetSync(last_dir);//↑/**/関数化
                
                state = MOVE;                                                               
                printf("state = MOVE\n");
            }
            break;
        case MOVE:
            //ev3_motor_set_power(left_motor, 45 + bias);
            //ev3_motor_set_power(right_motor, 45 - bias);
            Grid_steerAhead();

            // 指定位置までたどり着いたら状態遷移
            if( (cur_dis > target_dis)  && (grid_count < (GRID_NUM-1)) ) {
                last_dir = cur_dir;
                
                //motorをストップ
                //ev3_motor_stop(left_motor, true);
                //ev3_motor_stop(right_motor, true);
                Grid_steerStop();  
                //座標到達時の誤差計算
                Grid_noiseXY_calc();
                // 現在位置座標を更新
                cur_gridX = target_grid[grid_count].gridX;
                cur_gridY = target_grid[grid_count].gridY;

                // 次の座標までの方位,距離を格納する
                grid_count++;
                Grid_setDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
                Grid_setDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
                target_dis = Grid_getDistance();
                target_dir = Grid_getDirection();
                
                /* 計測器情報のリセット */
                /*
                //一旦オドメトリタスクをストップ&待ち
                stp_cyc(ODOMETRY_TASK_CYC);
                wait_msec(50);
                //last_dir = cur_dir;
                //printf("last_dir = cur_dir = %lf\n", cur_dir);
                // 距離値リセット
                odom_Distance_reset();

                // 再度,次座標への旋回を開始
                sta_cyc(ODOMETRY_TASK_CYC);
                //odom_Direction_setDirection(last_dir);//←これこのタイミングでいい？
                direction = last_dir;
                wait_msec(50);
                */
                odom_Distance_resetSync(last_dir);//↑/**/関数化
                odom_Coordinate_reset();
                state = TURN;
                printf("state = TURN\n");
            } else
                if( (cur_dis > target_dis) && (grid_count >= (GRID_NUM-1)) ) {
                    state = END;
                    printf("state = END\n");
                }
            break;
        case END:
            // モータを停止
            //ev3_motor_stop(left_motor, true);
            //ev3_motor_stop(right_motor, true);
            Grid_steerStop();  
            break;
        default:
            break;
        }
}


/* 初期化関数 */
void Grid_init() {
    //grid_distance = 0.0;
    //grid_direction = 0.0;
    noiseX = 0.0;
    noiseY = 0.0;    
    Grid_setDistance(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
    Grid_setDirection(cur_gridX, cur_gridY, target_grid[grid_count].gridX, target_grid[grid_count].gridY);
    target_dis = Grid_getDistance();
    target_dir = Grid_getDirection();
}

/* 座標aから座標bまでの移動距離を設定する関数 */
void Grid_setDistance(int aX, int aY, int bX, int bY) {
    //grid_distance = sqrt( pow((float)(bX-aX),2) + pow((float)(bY-aY),2) ) *  GRID_SIZE;
    grid_distanceX = (float)(bX-aX)* GRID_SIZE * edge + noiseX; 
    grid_distanceY = (float)(bY-aY)* GRID_SIZE + noiseY;
    grid_distance = sqrt( pow(grid_distanceX,2) + pow(grid_distanceY,2));
}

/* 座標aから座標bまでの移動距離を取得する関数 */
float Grid_getDistance() {
    printf("%lf\n", grid_distance);
    return grid_distance;
}

/* 目標座標の方位を設定する関数 */
void Grid_setDirection(int aX, int aY, int bX, int bY) {//これRコースのarctan大丈夫？
    float targetDir = 0.0;// 目標方位

    //　座標aから座標bへの方位（ラジアン）を取得
    //targetDir = atan2((float)(bY-aY), (float)(bX-aX));
    targetDir = atan2(grid_distanceY, grid_distanceX * edge);
    //ラジアンから度に変換
    targetDir = targetDir * 180.0 / PI;

    grid_direction = targetDir;
}

/* 目標座標の方位を取得する関数 */
float Grid_getDirection() {
    printf("%lf\n", grid_direction);
    return grid_direction;
}

void Grid_noiseXY_calc() {
    ODOM_XY cur_coordinate = odom_Coordinate_getCoordinate();
    noiseX = grid_distanceX - cur_coordinate.x;
    noiseY = grid_distanceY - cur_coordinate.y;
}

void Grid_setBias(){
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
    printf("angle_diff=%lf, bias=%d\n", angle_diff, bias);
}

void Grid_steerAhead(){
    ev3_motor_set_power(left_motor, 45 + bias);
    ev3_motor_set_power(right_motor, 45 - bias);
}

void Grid_steerTurn_P(){
    ev3_motor_set_power(left_motor, (-53 - bias) * edge);
    ev3_motor_set_power(right_motor, (53 + bias) * edge);
}

void Grid_steerTurn_N(){
    ev3_motor_set_power(left_motor, (53 + bias) * edge);
    ev3_motor_set_power(right_motor, (-53 - bias) * edge);
}

void Grid_steerStop(){
    ev3_motor_stop(left_motor, true);
    ev3_motor_stop(right_motor, true);
}
