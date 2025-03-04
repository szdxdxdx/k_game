#include <stdio.h>

#include "k_game.h"

static void step_1(void *unused) {

    printf("A：\t%2s\t%2s\t%2s\t%2s\t%2s\t%2s\n",
        k_key_pressed ('A') ? "按下" : "",
        k_key_held    ('A') ? "保持" : "",
        k_key_released('A') ? "松开" : "",
        k_key_idle    ('A') ? "空闲" : "",
        k_key_down    ('A') ? "DOWN" : "",
        k_key_up      ('A') ? "UP" : ""
    );
}

static int init_game(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.room_speed = 12;
    struct k_room *tmp_room = k_room_create(&config, NULL);
    k_room_add_step_callback(tmp_room, step_1, NULL);

    k_goto_room(tmp_room);

    return 0;
}

#if 0

int main(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    struct k_game_config config = K_GAME_CONFIG_INIT;
    config.fn_init = init_game;
    k_run_game(&config);

    return 0;
}

#endif
