#include <stdio.h>

#include "k_game.h"

static void step_1(void *unused) {

    printf("空格：\t%2s\t%2s\t%2s\t%2s\t%2s\t%2s\n",
        k_is_key_pressed (K_KEY_SPACE) ? "按下" : "",
        k_is_key_held    (K_KEY_SPACE) ? "保持" : "",
        k_is_key_released(K_KEY_SPACE) ? "松开" : "",
        k_is_key_idle    (K_KEY_SPACE) ? "空闲" : "",
        k_is_key_down    (K_KEY_SPACE) ? "DOWN" : "",
        k_is_key_up      (K_KEY_SPACE) ? "UP"   : ""
    );
}

void step_2(void *unused) {

}

static int init_game(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.room_speed = 12;
    config.room_name = "tmp room";
    struct k_room *tmp_room = k_create_room(&config, NULL);
    k_room_add_step_callback(tmp_room, step_1, NULL);

    k_goto_room(tmp_room);

    return 0;
}

#if 1

int main(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    struct k_game_config config = K_GAME_CONFIG_INIT;
    config.window_w = 260;
    config.window_h = 180;
    config.fn_init = init_game;

    k_run_game(&config);

    return 0;
}

#endif
