#ifdef __cplusplus
extern "C" {
#endif

/* 列挙型　シーン状態 */
enum SCENE_STATE{
    SCENE_LAP_1_STRAIGHT,
    SCENE_LAP_2_CURVE,
    SCENE_LAP_3_STRAIGHT,
    SCENE_LAP_4_CURVE,
    SCENE_LAP_5_STRAIGHT,
    SCENE_WLOOP_11_BLACK,
    SCENE_WLOOP_12_BLUE,
    SCENE_WLOOP_13_BLACK,
    SCENE_WLOOP_14_IN,
    SCENE_WLOOP_15_OUT,
    SCENE_WLOOP_21_BLACK,
    SCENE_WLOOP_22_BLUE,
    SCENE_WLOOP_23_BLACK,
    SCENE_WLOOP_24_IN,
    SCENE_WLOOP_25_OUT,
    SCENE_WLOOP_31_BLACK,
    SCENE_WLOOP_32_BLUE,
    SCENE_WLOOP_33_BLACK,
    SCENE_WLOOP_34_IN,
    SCENE_WLOOP_35_OUT,
    SCENE_WLOOP_41_BLACK,
    SCENE_WLOOP_42_BLUE,
    SCENE_WLOOP_43_BLACK,
    SCENE_WLOOP_44_IN,
    SCENE_WLOOP_45_OUT,
    SCENE_WLOOP_51_END,
    SCENE_WLOOP_52_END,
};

#define SCENE_START SCENE_LAP_1_STRAIGHT    /* 開始シーン */

/* 列挙型　エッジ状態 */
enum TRACE_MODE{
    EDGE_BLACK,             /* 黒線走行中 */
    EDGE_BLUE,              /* 青線走行中 */
    EDGE_IN,                /* エッジ切り替え（開始から黒線中央まで） */
    EDGE_OUT,               /* エッジ切り替え（黒線中央から反対エッジまで） */
};

/* 構造体 　シーン状態ごとのパラメータ */
struct SCENE_PARAM{
    char current_scene[32];         /* シーン状態名 */
    int base_speed;                 /* 基準前進速度 */
    float odom_target;              /* オドメトリ上限（走行距離がこの値を超えたらシーンを切り替える） */
    int rot_max;                    /* 旋回量上限（旋回量がこの値を超えたら、旋回量にこの値をセットする） */
    enum TRACE_MODE trace_mode;     /* エッジ状態 */
    float kp;                       /* KP */
    float ki;                       /* KI */
    float kd;                       /* KD */
    int edge;                       /* 走行エッジ */
    int rot_const;                  /* 固定値旋回量 */
    int curve_offset;               /* 曲線オフセット */
};

/* 関数プロトタイプ宣言 */
bool_t tracer_10ms_cycle();
void tracer_scene_transition(enum SCENE_STATE *scene_state);
void tracer_steering(enum SCENE_STATE scene_state);

#ifdef __cplusplus
}
#endif

