#ifdef __cplusplus
extern "C" {
#endif

/* シミュレータ */
#ifdef ETROBOC_SIM
    #define RGB_GAIN_RED        1.0     /* RGBセンサ補正値 */
    #define RGB_GAIN_GREEN      1.0     /* RGBセンサ補正値 */
    #define RGB_GAIN_BLUE       0.7     /* RGBセンサ補正値 */
/* RasPike実機 */
#else
    #define RGB_GAIN_RED        1.0     /* RGBセンサ補正値 */
    #define RGB_GAIN_GREEN      0.88     /* RGBセンサ補正値 */
    #define RGB_GAIN_BLUE       0.82     /* RGBセンサ補正値 */
#endif

/* 状態判定用パラメータ */
#ifdef ETROBOC_SIM  /* シミュレータ */
    #define S_THRE_LOW      0.3
    #define S_THRE_MID      0.5
    #define S_THRE_HIGH     0.7
    #define V_THRE_DARK     0.2
    #define V_THRE_LOW      0.3
    #define V_THRE_AVE      1.0
#else               /* RasPike実機 */
    #define S_THRE_LOW      0.5
    #define S_THRE_MID      0.5
    #define S_THRE_HIGH     0.5
    #define V_THRE_DARK     0.2
    #define V_THRE_LOW      0.2
    #define V_THRE_AVE      1.0
#endif


/* 関数プロトタイプ宣言 */
void color_update();
void color_get_hsv(int16_t *h_ref, int16_t *s_ref, int16_t *v_ref);
void adjust_rgb();
void rgb2hsv();

bool_t color_marker_is_detected(float s_threshold);
bool_t color_brighter_than_average(float v_threshold);

void color_print_rgbhsv();
void color_set_v_calib(int16_t v_min_ref, int16_t v_max_ref, int16_t v_ave_ref);
void color_set_s_calib(int16_t s_min_ref, int16_t s_max_ref, int16_t s_ave_ref);

#ifdef __cplusplus
}
#endif
