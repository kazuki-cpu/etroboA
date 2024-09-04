#ifdef __cplusplus
extern "C" {
#endif

/* 定数 */
#define LEFT_COURSE      1  /* 走行コースL*/
#define RIGHT_COURSE    -1  /* 走行コースR*/
#define LEFT_EDGE       1  /* 走行エッジL */
#define RIGHT_EDGE       -1  /* 走行エッジR */
#define NA               0  /* 無効値（Not Applicable） */

/* 状態判定用パラメータ */
#ifdef ETROBOC_SIM  /* シミュレータ */
    #define MARKER_COUNT    10
#else               /* RasPike実機 */
    #define MARKER_COUNT    10
#endif

/* 構造体　シーン状態ごとの走行パラメータ */
static struct SCENE_PARAM scene_param[] = {
    /* 現在シーン、前進速度、オドメトリ上限、旋回量上限、エッジ状態、KP、KI、KD、走行エッジ、旋回量一定値、曲線オフセット*/
#ifdef ETROBOC_SIM      /* シミュレータ */
    /* ラップタイム走行 */
    {"SCENE_LAP_1_STRAIGHT",    80,     24.0,   50,     EDGE_BLACK,     0.60,  0.0,     0.10,   LEFT_EDGE,   NA, 0  },   /* 直線 */
    {"SCENE_LAP_2_CURVE",       60,     6.0,    50,     EDGE_BLACK,     1.00,  0.0,     0.10,   LEFT_EDGE,   NA, 0  },   /* カーブ */
    {"SCENE_LAP_3_STRAIGHT",    80,     13.0,   50,     EDGE_BLACK,     0.60,  0.0,     0.10,   LEFT_EDGE,   NA, 0  },   /* 直線 */
    {"SCENE_LAP_4_CURVE",       60,     5.0,    50,     EDGE_BLACK,     1.00,  0.0,     0.10,   LEFT_EDGE,   NA, 0  },   /* カーブ */
    {"SCENE_LAP_5_STRAIGHT",    30,     3.0,    50,     EDGE_BLACK,     0.40,  0.0,     0.05,   LEFT_EDGE,   NA, 0  },   /* 直線 */
    /* ダブルループ１回目 */
    {"SCENE_WLOOP_11_BLACK",    20,     NA,     50,     EDGE_BLACK,     0.40,  0.0,     0.0,    LEFT_EDGE,   NA, 0  },   /* 黒色 */
    {"SCENE_WLOOP_12_BLUE",     20,     NA,     50,     EDGE_BLUE,      0.20,  0.0,     0.0,    LEFT_EDGE,   NA, 0  },   /* 青色 */
    {"SCENE_WLOOP_13_BLACK",    20,     0.2,    50,     EDGE_BLACK,     0.40,  0.0,     0.0,    LEFT_EDGE,   NA, 0  },   /* 黒色復帰 */
    {"SCENE_WLOOP_14_IN",       20,     NA,     50,     EDGE_IN,        NA,    NA,      NA,     LEFT_EDGE,   4 , NA },   /* エッジ切り替えIN */
    {"SCENE_WLOOP_15_OUT",      20,     NA,     50,     EDGE_OUT,       NA,    NA,      NA,     LEFT_EDGE,   3 , NA },   /* エッジ切り替えOUT */
    /* ダブルループ２回目 */
    {"SCENE_WLOOP_21_BLACK",    20,     NA,     50,     EDGE_BLACK,     0.40,  0.0,     0.0,    RIGHT_EDGE,  NA, 4  },   /* 黒色 */
    {"SCENE_WLOOP_22_BLUE",     20,     NA,     50,     EDGE_BLUE,      0.20,  0.0,     0.0,    RIGHT_EDGE,  NA, 4  },   /* 青色 */
    {"SCENE_WLOOP_23_BLACK",    20,     0.2,    50,     EDGE_BLACK,     0.40,  0.0,     0.0,    RIGHT_EDGE,  NA, 4  },   /* 黒色復帰 */
    {"SCENE_WLOOP_24_IN",       20,     NA,     50,     EDGE_IN,        NA,    NA,      NA,     RIGHT_EDGE,  8,  NA },   /* エッジ切り替えIN */
    {"SCENE_WLOOP_25_OUT",      20,     NA,     50,     EDGE_OUT,       NA,    NA,      NA,     RIGHT_EDGE,  6 , NA },   /* エッジ切り替えOUT */
    /* ダブルループ３回目 */
    {"SCENE_WLOOP_31_BLACK",    20,     NA,     50,     EDGE_BLACK,     0.40,  0.0,     0.0,    LEFT_EDGE,   NA, 2  },   /* 黒色 */
    {"SCENE_WLOOP_32_BLUE",     20,     NA,     50,     EDGE_BLUE,      0.20,  0.0,     0.0,    LEFT_EDGE,   NA, 8  },   /* 青色 */
    {"SCENE_WLOOP_33_BLACK",    20,     0.2,    50,     EDGE_BLACK,     0.40,  0.0,     0.0,    LEFT_EDGE,   NA, 8  },   /* 黒色復帰 */
    {"SCENE_WLOOP_34_IN",       20,     NA,     50,     EDGE_IN,        NA,    NA,      NA,     LEFT_EDGE,   2 , NA },   /* エッジ切り替えIN */
    {"SCENE_WLOOP_35_OUT",      20,     NA,     50,     EDGE_OUT,       NA,    NA,      NA,     LEFT_EDGE,   2 , NA },   /* エッジ切り替えOUT */
    /* ダブルループ４回目 */
    {"SCENE_WLOOP_41_BLACK",    20,     NA,     50,     EDGE_BLACK,     0.40,  0.0,     0.0,    RIGHT_EDGE,  NA, 4  },   /* 黒色 */
    {"SCENE_WLOOP_42_BLUE",     20,     NA,     50,     EDGE_BLUE,      0.20,  0.0,     0.0,    RIGHT_EDGE,  NA, 4  },   /* 青色 */
    {"SCENE_WLOOP_43_BLACK",    20,     0.2,    50,     EDGE_BLACK,     0.40,  0.0,     0.0,    RIGHT_EDGE,  NA, 4  },   /* 黒色復帰 */
    {"SCENE_WLOOP_44_IN",       20,     NA,     50,     EDGE_IN,        NA,    NA,      NA,     RIGHT_EDGE,  0 , NA },   /* エッジ切り替えIN */
    {"SCENE_WLOOP_45_OUT",      20,     NA,     50,     EDGE_OUT,       NA,    NA,      NA,     RIGHT_EDGE,  0 , NA },   /* エッジ切り替えOUT */
    /* 終了 */
    {"SCENE_WLOOP_51_END",      20,     5  ,    50,     EDGE_BLACK,     0.40,  0.0,     0.0,    LEFT_EDGE,   NA, 0  },   /* ダブルループ終了 */
    {"SCENE_WLOOP_52_END",      20,     NA ,    50,     EDGE_BLACK,     0.40,  0.0,     0.0,    LEFT_EDGE,   NA, 0  },   /* 次の競技に移動 */

#else       /* RasPike実機 */
    /* ラップタイム走行 */
    {"SCENE_LAP_1_STRAIGHT",    62,     22.0,   50,     EDGE_BLACK,     0.20,  0.0,     0.20,   LEFT_EDGE,   NA, 0  },   /* 直線 */
    {"SCENE_LAP_2_CURVE",       45,     6.0,    50,     EDGE_BLACK,     0.40,  0.0,     0.20,   LEFT_EDGE,   NA, 0  },   /* カーブ */
    {"SCENE_LAP_3_STRAIGHT",    62,     10.0,   50,     EDGE_BLACK,     0.20,  0.0,     0.20,   LEFT_EDGE,   NA, 0  },   /* 直線 */
    {"SCENE_LAP_4_CURVE",       45,     6.0,    50,     EDGE_BLACK,     0.40,  0.0,     0.20,   LEFT_EDGE,   NA, 0  },   /* カーブ */
    {"SCENE_LAP_5_STRAIGHT",    62,     2.0,    50,     EDGE_BLACK,     0.20,  0.0,     0.20,   LEFT_EDGE,   NA, 0  },   /* 直線 */
    /* ダブルループ１回目 */
    {"SCENE_WLOOP_11_BLACK",    50,     NA,     50,     EDGE_BLACK,     0.40,  0.0,     0.10,    LEFT_EDGE,   NA, 0  },   /* 黒色 */
    {"SCENE_WLOOP_12_BLUE",     50,     NA,     50,     EDGE_BLUE,      0.40,  0.0,     0.10,    LEFT_EDGE,   NA, 0  },   /* 青色 */
    {"SCENE_WLOOP_13_BLACK",    50,     0.2,    50,     EDGE_BLACK,     0.40,  0.0,     0.10,    LEFT_EDGE,   NA, 0  },   /* 黒色復帰 */
    {"SCENE_WLOOP_14_IN",       30,     NA,     50,     EDGE_IN,        NA,    NA,      NA,      LEFT_EDGE,   16 , NA },   /* エッジ切り替えIN */
    {"SCENE_WLOOP_15_OUT",      30,     NA,     50,     EDGE_OUT,       NA,    NA,      NA,      LEFT_EDGE,   16 , NA },   /* エッジ切り替えOUT */
    /* ダブルループ２回目 */
    {"SCENE_WLOOP_21_BLACK",    50,     NA,     50,     EDGE_BLACK,     0.40,  0.0,     0.10,    RIGHT_EDGE,  NA, 10  },   /* 黒色 */
    {"SCENE_WLOOP_22_BLUE",     50,     NA,     50,     EDGE_BLUE,      0.40,  0.0,     0.10,    RIGHT_EDGE,  NA, 10  },   /* 青色 */
    {"SCENE_WLOOP_23_BLACK",    50,     0.2,    50,     EDGE_BLACK,     0.40,  0.0,     0.10,    RIGHT_EDGE,  NA, 10  },   /* 黒色復帰 */
    {"SCENE_WLOOP_24_IN",       30,     NA,     50,     EDGE_IN,        NA,    NA,      NA,      RIGHT_EDGE,  20,  NA },   /* エッジ切り替えIN */
    {"SCENE_WLOOP_25_OUT",      30,     NA,     50,     EDGE_OUT,       NA,    NA,      NA,      RIGHT_EDGE,  20 , NA },   /* エッジ切り替えOUT */
    /* ダブルループ３回目 */
    {"SCENE_WLOOP_31_BLACK",    50,     NA,     50,     EDGE_BLACK,     0.40,  0.0,     0.10,    LEFT_EDGE,   NA, 6  },   /* 黒色 */
    {"SCENE_WLOOP_32_BLUE",     50,     NA,     50,     EDGE_BLUE,      0.40,  0.0,     0.10,    LEFT_EDGE,   NA, 12  },   /* 青色 */
    {"SCENE_WLOOP_33_BLACK",    50,     0.2,    50,     EDGE_BLACK,     0.40,  0.0,     0.10,    LEFT_EDGE,   NA, 12  },   /* 黒色復帰 */
    {"SCENE_WLOOP_34_IN",       50,     NA,     50,     EDGE_IN,        NA,    NA,      NA,     LEFT_EDGE,   16 , NA },   /* エッジ切り替えIN */
    {"SCENE_WLOOP_35_OUT",      50,     NA,     50,     EDGE_OUT,       NA,    NA,      NA,     LEFT_EDGE,   16 , NA },   /* エッジ切り替えOUT */
    /* ダブルループ４回目 */
    {"SCENE_WLOOP_41_BLACK",    50,     NA,     50,     EDGE_BLACK,     0.40,  0.0,     0.10,    RIGHT_EDGE,  NA, 10  },   /* 黒色 */
    {"SCENE_WLOOP_42_BLUE",     50,     NA,     50,     EDGE_BLUE,      0.40,  0.0,     0.10,    RIGHT_EDGE,  NA, 10  },   /* 青色 */
    {"SCENE_WLOOP_43_BLACK",    50,     0.2,    50,     EDGE_BLACK,     0.40,  0.0,     0.10,    RIGHT_EDGE,  NA, 10  },   /* 黒色復帰 */
    {"SCENE_WLOOP_44_IN",       50,     NA,     50,     EDGE_IN,        NA,    NA,      NA,     RIGHT_EDGE,  16 , NA },   /* エッジ切り替えIN */
    {"SCENE_WLOOP_45_OUT",      50,     NA,     50,     EDGE_OUT,       NA,    NA,      NA,     RIGHT_EDGE,  16 , NA },   /* エッジ切り替えOUT */
    /* 終了 */
    {"SCENE_WLOOP_51_END",      55,     100,    50,     EDGE_BLACK,     0.40,  0.0,     0.10,    LEFT_EDGE,   NA, 0  },   /* ダブルループ終了 */
    {"SCENE_WLOOP_52_END",      0,     NA ,    50,     EDGE_BLACK,     0.40,  0.0,     0.0,    LEFT_EDGE,   NA, 0  },   /* 次の競技に移動 */
#endif
};

#ifdef __cplusplus
}
#endif
