#include <stdio.h>

#include "k_game.h"

static void fn_step(void *unused) {

    static int count = 0;
    printf("step: %3d | [A] %7s %4s %8s %4s %4s %2s\n",
        ++count,
        k_key_pressed ('A') ? "pressed"  : "",
        k_key_held    ('A') ? "held"     : "",
        k_key_released('A') ? "released" : "",
        k_key_idle    ('A') ? "idle"     : "",
        k_key_down    ('A') ? "down"     : "",
        k_key_up      ('A') ? "up"       : ""
    );
}

static int init_game(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.room_speed = 20;
    struct k_room *room = k_room_create(&config, NULL);

    k_room_add_step_callback(room, fn_step, NULL);

    k_goto_room(room);
    return 0;
}

#if 1

int main(int argc, char **argv) {

    struct k_game_config config = K_GAME_CONFIG_INIT;
    config.fn_init = init_game;
    k_game_run(&config);

    return 0;
}

#endif
