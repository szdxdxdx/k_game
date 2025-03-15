#include "SDL_mixer.h"

#include "k_game.h"

#include "./k_sound.h"
#include "k_log.h"

struct k_sound_BGM *k_sound_BGM_load(const char *filepath) {

    struct k_sound_BGM *sound = k_malloc(sizeof(struct k_sound_BGM));
    if (NULL == sound)
        return NULL;

    Mix_Music *music = Mix_LoadMUS(filepath);
    if (NULL == music) {
        k_log_error("Mix_LoadMUS(): %s", Mix_GetError());
        k_free(sound);
        return NULL;
    }

    sound->music = music;
    k__sound_registry_add_BGM(sound);
    return sound;
}

void k_sound_BGM_release(struct k_sound_BGM *sound) {

    if (NULL == sound)
        return;

    k__sound_registry_del_BGM(sound);
    Mix_FreeMusic(sound->music);
    k_free(sound);
}

int k_sound_BGM_loop(struct k_sound_BGM *sound, int loops) {

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
