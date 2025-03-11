#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "k_game.h"

static void room_step(void *data) {

}

static int init(void) {

    struct k_room_config room_config = K_ROOM_CONFIG_INIT;
    struct k_room *room = k_room_create(&room_config, NULL);
    k_room_add_step_callback(room, room_step, NULL);

    k_goto_room(room);
    return 0;
}

#if 0

int main(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    struct k_game_config game_config = K_GAME_CONFIG_INIT;
    game_config.fn_init = init;
    k_game_run(&game_config);

    return 0;
}

#endif
