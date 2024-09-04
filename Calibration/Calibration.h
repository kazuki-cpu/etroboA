#ifdef __cplusplus
extern "C" {
#endif

#ifdef ETROBOC_SIM  /* シミュレータ */
    #define S_MIN 2 
    #define V_MIN 5
    #define S_MAX 241
    #define V_MAX 107
    #define S_AVE 121
    #define V_AVE 56
#else               /* RasPike実機 */
    #define S_MIN 44
    #define V_MIN 11
    #define S_MAX 255
    #define V_MAX 184
    #define S_AVE 149
    #define V_AVE 97
#endif

/* 関数プロトタイプ宣言 */
void v_calibration();                       /* V値キャリブレーション関数 */
void s_calibration();                       /* S値キャリブレーション関数 */
void skip_calibration();
void v_sensor_calib_task(intptr_t unused);  /* 光センサV値キャリブレーションタスク）(10msec周期) */
void s_sensor_calib_task(intptr_t unused);  /* 光センサS値キャリブレーションタスク）(10msec周期) */
void calib_movement();                      /* キャリブレーション */
#ifdef __cplusplus
}
#endif
