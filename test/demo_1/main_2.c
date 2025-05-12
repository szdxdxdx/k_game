#include <stdio.h>
#include <stdlib.h>

#include "k_game.h"

#include "./room/yx_room_1.h"

static int yx_game_on_start(void) {
    struct k_room *room_1 = yx_room_1_create(); /* 在初始化时创建房间 */
    if (NULL == room_1)
        return -1; /* 若房间创建失败，则返回 -1，表示游戏初始化失败，框架会自行回滚 */

    k_room_nav_goto(room_1); /* 若房间创建成功，则需要在初始化结束前指定一个初始房间，这样才能真正开始游戏 */
    return 0; /* 函数返回 0 表示初始化成功，框架继续执行游戏 */
}

int main(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    struct k_game_config config = K_GAME_CONFIG_INIT;
    config.window_w     = 640; /* 指定游戏窗口的大小 */
    config.window_h     = 480;
    config.window_title = "demo 1"; /* 指定游戏窗口的标题 */
    config.on_start     = yx_game_on_start; /* 游戏初始化时执行的回调 */
    return k_game_run(&config); /* 运行游戏 */
}




