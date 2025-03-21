#include <stdio.h>
#include <stdlib.h>

#include "./sprite/_public.h"
#include "./room/my_room.h"

static void load_sprite(void) {
    my_spr_player_load();
    my_spr_player_bullet_load();
    my_spr_enemy_load();

    my_spr_ynx_load();
    my_spr_bubble_load();
}

static void load_sound(void) {
    struct k_sound_BGM *bgm = k_sound_BGM_load("./demo_1/sound/bgm.wav");
    k_sound_BGM_loop(bgm, INT_MAX);
}

static int fn_init_game(void) {
    load_sprite();
    load_sound();

    struct k_room *room = my_room_1_create();

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
