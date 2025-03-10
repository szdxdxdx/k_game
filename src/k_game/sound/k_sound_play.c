#include "k_log.h"

#include "./k_sound.h"

static inline int k__sound_play(struct k_sound *sound) {

    Mix_Music *music = sound->music;

    if (0 != Mix_PlayMusic(music, 1)) {
        k_log_error("Mix_PlayMusic(): %s", Mix_GetError());
        return -1;
    }

    return 0;
}

int k_sound_play(struct k_sound *sound) {

    if (NULL == sound) {
        return -1;
    } else {
        return k__sound_play(sound);
    }
}
