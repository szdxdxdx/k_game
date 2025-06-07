#ifndef K_GAME_RUN_H
#define K_GAME_RUN_H

#include <stddef.h>

/** \brief 游戏运行所需的配置参数 */
struct k_game_config {

    /** \brief 游戏窗口的标题 */
    const char *window_title;

    /** \brief 游戏窗口的宽高 */
    int window_w;
    int window_h;

    /**
     * \brief 游戏初始化阶段的回调函数
     *
     * 此回调在游戏环境初始化完成后执行，
     * 用于加载资源、创建房间、注册组件类型等初始化操作。
     *
     * 你必须实现该回调，否则游戏无法启动。
     */
    int (*on_start)(void);

    /**
     * \brief 游戏结束时执行的回调
     *
     * 游戏结束，即将退出时执行此回调。
     * 之后 k_game 会释放资源、销毁房间等。
     *
     * 此回调是可选的，设为 `NULL` 则不执行回调。
     */
    void (*on_end)(void);
};

/** \brief 游戏运行所需的配置参数的默认值 */
#define K_GAME_CONFIG_INIT \
{ \
    .window_title = "", \
    .window_h = 480,  \
    .window_w = 640,  \
    .on_start = NULL, \
    .on_end   = NULL, \
}

/**
 * \brief 启动并运行游戏
 *
 * 本函数为 k_game 框架的入口函数。
 *
 * 不必理会此函数的返回值。
 */
int k_game_run(const struct k_game_config *config);

#endif
