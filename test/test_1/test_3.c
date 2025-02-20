#include <stdio.h>

#include "k_game.h"

static int create_room(struct k_room *room, void *unused) {

    return 0;
}

static int init_game(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.room_speed = 1;
    config.room_name = "tmp room";
    config.fn_create = create_room;
    struct k_room *tmp_room = k_create_room(&config, NULL);

    k_goto_room(tmp_room);

    return 0;
}

#if 1

int main(int argc, char **argv) {
    setbuf(stdout, NULL);

    struct k_game_config config = K_GAME_CONFIG_INIT;

    config.fn_init = init_game;

    k_run_game(&config);

    return 0;
}

#endif
