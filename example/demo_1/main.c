#include <stdio.h>

#include "k/game.h"

static int create_room(const struct k_room *room) {

    printf("\ncreate my room\n\n");
    return 3;
}

static void destroy_room(const struct k_room *room) {

    printf("\ndestroy my room\n\n");
}

static void room_step(const struct k_room *room) {

}

static int setup_game(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.room_name = "tmp room";
    config.fn_create_event  = create_room;
    config.fn_destroy_event = destroy_room;
    config.fn_step_event    = room_step;
    size_t room_idx = k_create_room(&config);

    k_goto_room(room_idx);

    return 0;
}

#if 1

int main(int argc, char **argv) {
    setbuf(stdout, NULL);

    struct k_game_config config = K_GAME_CONFIG_INIT;
    config.fn_setup_game = setup_game;

    k_run_game(&config);

    return 0;
}

#endif
