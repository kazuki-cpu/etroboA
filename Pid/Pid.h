#ifdef __cplusplus
extern "C" {
#endif

#define S_AVE_OFFSET 0.8

/* 関数プロトタイプ宣言 */
void pid_reset();
int get_pid_v(float kp, float ki, float kd);
int get_pid_s(float kp, float ki, float kd);

#ifdef __cplusplus
}
#endif
