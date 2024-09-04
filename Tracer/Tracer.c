#pragma once
#include "app.h"
#include "Color.h"
#include "Grid.h"
#include "Odometry.h"
#include "Tracer.h"

#define SENSOR_COORDINATE_DIFF 40 //カラーセンサーと車輪軸までの距離

typedef enum {
    WHITE,
    BLACK,
    EDGE,
    MIDDLE,
    BLUE,
    CENTERLINE,
    CENTER
} DETECT_TARGET;
static DETECT_TARGET target = WHITE;

int bias;
extern float angle_diff;

void tracer_task(intptr_t unused) {
    color_update();
    Grid_setBias();
    
    switch(target) {        
        case WHITE:   /*エッジ外側の白を検知するまで旋回、白検知したらエッジの黒検知(DETECT_TARGET = BLACK)移行*/
            int16_t v = color_get_v();
            int16_t v_ave = color_get_v_ave();
            
                if(v > v_ave) {
                    target = BLACK;
                }
                else{
                    Grid_steerTurn_P();
                }
            break;
        case BLACK:   /*エッジの黒検知するまで旋回、黒検知したらエッジ斜め横断(DETECT_TARGET = EDGE)移行。この時カラーセンサーの座標を(-1,0)としてオドメトリリセット*/
            int16_t v = color_get_v();
            int16_t v_ave = color_get_v_ave();

                if(v < v_ave) {;
                    target = EDGE;
                    odom_Distance_resetSync(0.0);
                }
                else{
                    Grid_steerTurn_N();
                }
            break;
        case EDGE:   /*逆エッジ超えた白検知するまで直進、白検知したら入射角と座標現在の推定し次の状態に移行*/
            int16_t v = color_get_v();
            int16_t v_ave = color_get_v_ave();

                if(v > v_ave) {
                    target = MIDDLE;
                    Grid_steerStop();
                    //入射方位(現在のロボット方位)の推定＆設定
                    float incide_dis = odom_Distance_getDistance();
                    float incide_dir = incideDirection_calc(incide_dis, 20);
                    odom_Direction_setDirection(incide_dir);
                    //現在のロボット座標の推定＆設定
                    float x = (incide_dis - SENSOR_COORDINATE_DIFF) * cos(incide_dir) - 1;
                    float y = (incide_dis - SENSOR_COORDINATE_DIFF) * sin(incide_dir);
                    odom_Coordinate_setXY(x, y);
                }
                else{
                    Grid_steerAhead();
                }
            break;
        case MIDDLE:
            if(odom_Direction_getDirection() < 90.0) {
                Grid_steerTurn_P();
            } 
            else {
                Grid_steerTurn_N();
            }
            
            // 指定方位の一定範囲内に収まったら,移動開始
            if( (odom_Direction_getDirection() > (89.0)) && (odom_Direction_getDirection() < (91.0)) ) {;
                target = BLUE;                                                                                                  
            }
            break;
        case BLUE:
            int16_t s = color_get_s();
            int16_t v = color_get_v();
            
                if(s > S_AVERAGE && v > V_DARK){       /* 青を検知したら */
                    target = CENTERLINE;             /* 次の走行状態に遷移 */
                    ODOM_XY cur_coordinate = odom_Coordinate_getCoordinate();
                    target_dir = incideDirection_calc(50.0, -cur_coordinate.x);
                }
                else{
                    Grid_steerAhead();
                }
            break;
        case CENTERLINE:
            // 指定方位まで旋回する
            if(cur_dir < target_dir) {
                Grid_steerTurn_P();
            } else {
                Grid_steerTurn_N();
            }
            // 指定方位の一定範囲内に収まったら,移動開始
            if( (cur_dir > (target_dir-1.0)) && (cur_dir < (target_dir+1.0)) ) {
                target = CENTER;
                target_dis = 50.0;
                odom_Distance_resetSync(cur_dir);
            }      
            break;
        case CENTER:
            Grid_steerAhead();
            if( (cur_dis > target_dis) {
                Grid_steerStop();//task終了
            }
            break;
        default:
            break;
    }
}

float incideDirection_calc(float hypotenuse, float neighbor){
    //return arccos(20.0/odom_Distance_getDistance());
    return arccos(neighbor/hypotenuse);
}
