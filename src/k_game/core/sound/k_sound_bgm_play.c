#include "k_log.h"

#include "./k_sound_bgm.h"

int k_sound_bgm_loop(struct k_sound_bgm *sound, int loops) {

    if (NULL == sound)
        return -1;

    if (loops <= 0)
        return 0;

    if (0 != Mix_PlayMusic(sound->music, loops)) {
        k_log_error("Mix_PlayMusic(): %s", Mix_GetError());
        return -1;
    }

    return 0;
}
