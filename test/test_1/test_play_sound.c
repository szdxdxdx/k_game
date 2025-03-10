#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "k_game.h"

static struct k_sound *sound_bgm;
static struct k_sound *sound_yx;
static struct k_sound *sound_hhddd;

static void room_step(void *data) {

    if (k_key_pressed('H')) {
        k_sound_loop(sound_hhddd, INT_MAX);
    }

    if (k_key_pressed('Y')) {
        k_sound_play(sound_yx);
    }
}

static int init(void) {

    struct k_room_config room_config = K_ROOM_CONFIG_INIT;
    struct k_room *room = k_room_create(&room_config, NULL);
    k_room_add_step_callback(room, room_step, NULL);

    sound_bgm = k_sound_load("assets/bgm.wav", K_SOUND_BGM);
    sound_yx = k_sound_load("assets/sfx.mp3", K_SOUND_SFX);
    sound_hhddd = k_sound_load("assets/DJMZJ_HHDDD.mp3", K_SOUND_BGM);

    k_sound_set_name(sound_hhddd, "hhddd");

    k_sound_loop(sound_bgm, INT_MAX);

    k_goto_room(room);
    return 0;
}

#if 1

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
