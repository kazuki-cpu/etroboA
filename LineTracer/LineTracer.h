#ifdef __cplusplus
extern "C" {
#endif

/* カラーセンサの輝度設定 */
#define WHITE_BRIGHTNESS  (180)
#define BLACK_BRIGHTNESS  (10) 

/* 走行パラメータ */
#define BASE_SPEED        (30)  /* 基準速度 */
#define ROTATE_COEF     (0.4F)  /* 旋回量の係数 */

/* ライントレースエッジ切り替え */
#define LEFT_EDGE         (-1) 
#define RIGHT_EDGE        (1) 

#ifdef __cplusplus
}
#endif
