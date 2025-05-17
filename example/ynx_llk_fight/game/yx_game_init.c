#include "k_game.h"

#include "game/yx_game_init.h"

#include "sprite/yx_spr.h"
#include "room/yx_room_arena.h"
#include "room/yx_room_title.h"
#include "room/room_1/yx_room_1.h"

static int on_game_start(void) {

    if (0 != yx_spr_load()) return -1;

    {
        struct k_image *img = k_image_load("./demo_1/image/szdxdxdx.jpg");
        k_image_set_name(img, "szdxdxdx");
    }

    {
        struct k_sound_bgm *bgm = k_sound_bgm_load("./demo_1/sound/bgm.wav");
        k_sound_bgm_set_name(bgm, "bgm");
    }

    {
        // struct k_room *room_empty = yx_room_empty_create();
        //struct k_room *room_arena = yx_room_arena_create();
        //struct k_room *room_title = yx_room_title_create();

        if (NULL == yx_room_1_create())
            return -1;

        k_room_nav_push(yx__room_1);
    }

    return 0;
}

int yx_game_run(void) {

    struct k_game_config config = K_GAME_CONFIG_INIT;
    config.window_w = 640;
    config.window_h = 480;
    config.window_title = "ynx! llk! fight!";
    config.on_start = on_game_start;

    return k_game_run(&config);
}
