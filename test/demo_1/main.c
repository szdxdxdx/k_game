#include <stdio.h>
#include <stdlib.h>

#include "k_game.h"
#include "k_webui.h"

#include "./sprite/yx_spr.h"
#include "./room/yx_room.h"

static void create_room(void) {

    // struct k_room *room = yx_create_arena_room();
    struct k_room *room = yx_create_empty_room();

    k_room_goto(room);
}

static int fn_init_game(void) {

    struct k_image *szdxdxdx = k_image_load("./demo_1/image/szdxdxdx.jpg");
    k_image_set_name(szdxdxdx, "szdxdxdx");

    yx_load_spr_ynx();
    yx_load_spr_liliko();
    yx_load_spr_bubble();
    yx_load_spr_iris();

    struct k_sound_bgm *bgm = k_sound_bgm_load("./demo_1/sound/bgm.wav");
    k_sound_bgm_loop(bgm, INT_MAX);

    create_room();

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
    config.fn_init = fn_init_game;

    k_game_run(&config);

    k_webui_close();

    return 0;
}

#endif
