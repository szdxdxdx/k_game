#include "k_game.h"

#include "yx_game_init.h"
#include "room/yx_room_1.h"

static int yx__game_on_start(void) {

    if (NULL == yx_room_1_create())
        return -1;

    k_room_nav_goto(yx_room_1);
    return 0;
}

int yx_game_run(void) {

    struct k_game_config config = K_GAME_CONFIG_INIT;
    config.window_w     = 640;
    config.window_h     = 480;
    config.window_title = "ynx! llk! fight!";
    config.on_start     = yx__game_on_start;

    return k_game_run(&config);
}
