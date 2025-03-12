#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "k_game.h"

struct k_sound_BGM *bgm, *ddd;
struct k_sound_SFX *ynx;

static void room_step(void *data) {

    if (k_key_pressed('Y'))
        k_sound_SFX_play(ynx);

    if (k_key_pressed('H'))
        k_sound_BGM_loop(ddd, INT_MAX);
}

static void fn_enter(void) {
    struct k_room *room = k_get_current_room();

    bgm = k_sound_BGM_load("./assets/bgm.wav");
    ddd = k_sound_BGM_load("./assets/ddd.mp3");
    ynx = k_sound_SFX_load("./assets/ynx.mp3");

    k_sound_BGM_loop(bgm, INT_MAX);

    k_room_add_step_callback(room, NULL, room_step);
}

static int init(void) {

    struct k_room_config room_config = K_ROOM_CONFIG_INIT;
    room_config.fn_enter = fn_enter;
    struct k_room *room = k_room_create(&room_config, NULL);

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
