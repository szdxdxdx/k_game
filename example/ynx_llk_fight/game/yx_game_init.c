#include "k_game.h"

#include "game/yx_game_init.h"

#include "sound/yx_sound.h"
#include "sprite/yx_spr.h"
#include "room/yx_room_arena.h"
#include "room/yx_room_title.h"
#include "room/room_1/yx_room_1.h"
#include "image/yx_image.h"
#include "k_webui.h"
#include "room/yx_room_empty.h"

static int on_game_start(void) {

    if (0 != yx_sound_load_on_game_start()) return -1;
    if (0 != yx_image_load_on_game_start()) return -1;
    if (0 != yx_spr_load_on_game_start())   return -1;

    {
        // struct k_room *room_empty = yx_room_empty_create();
        // struct k_room *room_arena = yx_room_arena_create();
        if (NULL == yx_room_title_create())
            return -1;

        if (NULL == yx_room_1_create())
            return -1;
    }

    k_room_nav_push(yx_room_title);
    return 0;
}

int yx_game_run(void) {

    k_webui_init();

    struct k_game_config config = K_GAME_CONFIG_INIT;
    config.window_w = 768;
    config.window_h = 576;
    config.window_title = "ynx! llk! fight!";
    config.on_start = on_game_start;

    return k_game_run(&config);
}
