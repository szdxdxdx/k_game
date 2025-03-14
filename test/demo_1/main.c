
#include <stdio.h>
#include <stdlib.h>

#include "./demo_1.h"

static int fn_init_game(void) {

    my_load_sprite_asset();

    struct k_room *room = my_create_room_1();

    k_goto_room(room);
    return 0;
}

int main(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    struct k_game_config config = K_GAME_CONFIG_INIT;
    config.window_title = "demo 1";
    config.fn_init = fn_init_game;

    return k_game_run(&config);
}
