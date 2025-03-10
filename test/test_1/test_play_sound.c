
#if 0

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_mixer.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "k_game.h"

static struct k_sound *sound_1, *sound_2;
static Mix_Music *music_1;
static Mix_Music *music_2;
static struct k_room_callback *player_hhddd;

static void room_step(void *data) {

    if (k_key_pressed('H')) {
        k_sound_play(sound_2);
    }
}

static int init(void) {

    struct k_room_config room_config = K_ROOM_CONFIG_INIT;
    struct k_room *room = k_room_create(&room_config, NULL);
    k_room_add_step_callback(room, room_step, NULL);

    sound_1 = k_sound_load(NULL, "assets/bgm.wav", K_SOUND_BGM);
    sound_2 = k_sound_load(NULL, "assets/sfx.mp3", K_SOUND_SFX);

    k_sound_play(sound_1);

    k_goto_room(room);
    return 0;
}

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
