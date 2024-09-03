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
    Grid_setBias();
    
    switch(target) {        
        case WHITE:
            int16_t v = color_get_v();
            int16_t v_ave = color_get_v_ave();
            /*エッジ外側の白を検知するまで旋回、白検知したらエッジの黒検知(DETECT_TARGET = BLACK)移行*/
                if(v > v_ave) {
                    target = BLACK;
                }
                else{
                    Grid_steerTurn_P();
                }
            break;
        case BLACK:
            int16_t v = color_get_v();
            int16_t v_ave = color_get_v_ave();
            
            /*エッジの黒検知するまで旋回、黒検知したらエッジ斜め横断(DETECT_TARGET = EDGE)移行。この時カラーセンサーの座標を(-1,0)としてオドメトリリセット*/
                if(v < v_ave) {;
                    target = EDGE;
                    odom_Distance_resetSync(0.0);
                }
                else{
                    Grid_steerTurn_N();
                }
            break;
        case EDGE:
            int16_t v = color_get_v();
            int16_t v_ave = color_get_v_ave();
            /*逆エッジ超えた白検知するまで直進、白検知したら入射角と座標現在の推定し次の状態に移行*/
                if(v > v_ave) {
                    target = MIDDLE;
                    Grid_steerStop();
                    //入射方位(現在のロボット方位)の推定
                    float incide_dis = odom_Distance_getDistance();
                    float incide_dir = incideDirection_calc(incide_dis, 20);
                    odom_Direction_setDirection(incide_dir);
                    //現在のロボット座標の推定
                    incide_dis - ;
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
                    target = CENTER;             /* 次の走行状態に遷移 */
                    
                }
                else{
                    Grid_steerAhead();
                }
            break;
        case CENTER:
               if(white_count>10){
            
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

float incideDirection_calc(float hypotenuse, float neighbor){
    //return arccos(20.0/odom_Distance_getDistance());
    return arccos(neighbor/hypotenuse);
}
