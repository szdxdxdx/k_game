#include "SDL_mixer.h"

#include "k_game_alloc.h"

#include "./k_sound.h"
#include "k_log.h"
#if 0
struct k_sound *k__sound_BGM_load(const char *filepath) {

    //Mix_Music *music = Mix_LoadMUS(filepath);

}

void k__sound_BGM_release(struct k_sound *sound) {

    //Mix_FreeMusic(sound->music);
}

int k__sound_BGM_loop(struct k_sound *sound, int loops) {

    if (0 != Mix_PlayMusic(sound->music, loops)) {
        k_log_error("Mix_PlayMusic(): %s", Mix_GetError());
        return -1;
    } else {
        return 0;
    }
}


#endif