#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "./sprite/_public.h"
#include "./room/_public.h"

static void load_sprite(void) {
    yx_load_spr_ynx();
    yx_load_spr_liliko();
    yx_load_spr_bubble();
    yx_load_spr_iris();
}

static void load_sound(void) {
    struct k_sound_BGM *bgm = k_load_sound_BGM("./demo_1/sound/bgm.wav");
    k_loop_sound_BGM(bgm, INT_MAX);
}

static void create_room(void) {

    struct k_room *room = yx_room_1_create();

    k_goto_room(room);
}

static int fn_init_game(void) {
    load_sprite();
    load_sound();
    create_room();
    return 0;
}

int main(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    struct k_game_config config = K_GAME_CONFIG_INIT;
    config.window_title = "demo 1";
    config.fn_init = fn_init_game;

    return k_run_game(&config);
}
