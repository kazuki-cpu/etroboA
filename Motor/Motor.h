#ifdef __cplusplus
extern "C" {
#endif

/* 関数プロトタイプ宣言 */
void motor_set_edge(int edge);
void motor_set_range(int rot_max);
void motor_set_power(int base_speed, int rotation);
void motor_set_power_inout(enum TRACE_MODE trace_mode, int edge);
void motor_print_fwd();
void motor_print_rot();

#ifdef __cplusplus
}
#endif
