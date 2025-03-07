#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_mixer.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "k_game.h"

static Mix_Music *music_1;
static Mix_Music *music_2;
static struct k_room_callback *player_hhddd;

static void room_step(void *data) {

    if (k_key_pressed('H')) {
        Mix_PlayMusic(music_2, 1);

        k_room_del_callback(player_hhddd);
        player_hhddd = NULL;
    }
}

static void room_enter(void *data) {
    struct k_room *room = k_get_current_room();

    music_1 = Mix_LoadMUS("assets/tmp.wav");
    Mix_PlayMusic(music_1, -1);

    music_2 = Mix_LoadMUS("assets/DJMZJ_HHDDD.mp3");
    player_hhddd = k_room_add_step_callback(room, room_step, NULL);
}

static int init(void) {

    struct k_room_config room_config = K_ROOM_CONFIG_INIT;
    struct k_room *room = k_room_create(&room_config, NULL);
    k_room_add_enter_callback(room, room_enter, NULL);

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
