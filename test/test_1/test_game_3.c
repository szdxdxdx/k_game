#include <stdio.h>

#include "k_game.h"

static void step(void *unused) {

    printf("\033[0G                   \033[0G");

    if (k_is_key_pressed(K_KEY_SPACE)) {
        printf("按下 ");
    }

    if (k_is_key_released(K_KEY_SPACE)) {
        printf("松开 ");
    }

    if (k_is_key_held(K_KEY_SPACE)) {
        printf("保持 ");
    }

    if (k_is_key_idle(K_KEY_SPACE)) {
        printf("空闲 ");
    }
}

static int init_game(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.room_speed = 5;
    config.room_name = "tmp room";
    struct k_room *tmp_room = k_create_room(&config, NULL);
    k_room_add_step_callback(tmp_room, step, NULL);

    k_goto_room(tmp_room);

    return 0;
}

#if 1

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
