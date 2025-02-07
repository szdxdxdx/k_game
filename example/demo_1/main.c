#include <stdio.h>

#include "k/game.h"

struct my_room {
    uint32_t timer;
    int count;
};

static int create_room(const struct k_room *room) {
    printf("\ncreate my room\n\n");

    return 0;
}

static int entry_room(const struct k_room *room) {
    printf("\nentry my room\n\n");
    return 0;
}

static void room_step(const struct k_room *room) {
}

static int init_game(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.room_name = "tmp room";
    config.data_size = sizeof(struct my_room);
    config.fn_create = create_room;
    config.fn_enter  = entry_room;
    config.fn_step   = room_step;
    struct k_room *tmp_room = k_create_room(&config);

    k_goto_room(tmp_room);

    return 1;
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
