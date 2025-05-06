#include <stdio.h>
#include <stdlib.h>

#include "k_game.h"
#include "k_webui.h"

#include "./sprite/yx_spr.h"

#include "./room/yx_room_arena.h"
#include "./room/yx_room_empty.h"
#include "./room/yx_room_title.h"

static int on_game_start(void) {

    {
        struct k_image *img = k_image_load("./demo_1/image/szdxdxdx.jpg");
        k_image_set_name(img, "szdxdxdx");
    }
    {
        yx_spr_ynx_load();
        yx_spr_liliko_load();
        yx_spr_bubble_load();
        yx_spr_iris_load();
    }
    {
        struct k_sound_bgm *bgm = k_sound_bgm_load("./demo_1/sound/bgm.wav");
        k_sound_bgm_set_name(bgm, "bgm");
    }
    {
        // struct k_room *room_empty = yx_room_empty_create();
        struct k_room *room_arena = yx_room_arena_create();

        struct k_room *room_title = yx_room_title_create();

        k_room_nav_goto(room_title);
    }

    return 0;
}

#if 1

int main(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    k_webui_init();

    struct k_game_config config = K_GAME_CONFIG_INIT;
    config.window_title = "demo 1";
    config.on_start = on_game_start;

    k_game_run(&config);

    k_webui_close();

    return 0;
}

#endif
