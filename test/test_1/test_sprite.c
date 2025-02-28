#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "k_game.h"

void room_step(void *data) {

    printf("1\n");
}

int init_game(void) {

    struct k_room_config room_config = K_ROOM_CONFIG_INIT;
    room_config.room_name = "room_1";
    struct k_room *room = k_create_room(&room_config, NULL);
    k_room_add_step_callback(room, room_step, NULL);

    k_create_room(&room_config, NULL);

    k_goto_room(room);
    return 0;
}

#if 1

int main(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    struct k_game_config game_config = K_GAME_CONFIG_INIT;
    game_config.fn_init = init_game;
    k_run_game(&game_config);

    return 0;
}

#endif
