#ifdef __cplusplus
extern "C" {
#endif

/* ラップタイム走行 *//* タブルループNEO *//* デブリリムーバル *//* スマートキャリー *//* 停車処理 */
enum MANAGE_STATE{
    TRACER,
    DEBRI,
    CARRY,
    PARKING,
};

/* 関数プロトタイプ宣言 */
void start_manager(enum MANAGE_STATE manage_state_ref);

#ifdef __cplusplus
}
#endif
