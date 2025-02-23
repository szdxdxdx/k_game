#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "k_game.h"

struct my_object {
    int data_1;
    int data_2;
};

static int create_room(struct k_room *room, void *params) {

    k_create_object(sizeof(struct my_object));

    return 0;
}

static int init(void) {

    struct k_room_config room_config = K_ROOM_CONFIG_INIT;
    room_config.room_name = "room";
    room_config.fn_create = create_room;
    struct k_room *room = k_create_room(&room_config, NULL);

    k_goto_room(room);
    return 0;
}

#if 1

int main(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    struct k_game_config game_config = K_GAME_CONFIG_INIT;
    game_config.window_w = 400;
    game_config.window_h = 360;
    game_config.fn_init = init;

    k_run_game(&game_config);

    return 0;
}

#endif
