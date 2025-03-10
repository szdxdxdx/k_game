#include "k_game_alloc.h"

#include "./k_sound.h"

struct k_sound *k_sound_load(const char *sound_name, const char *filepath, enum k_sound_type sound_type) {

    struct k_sound *sound = k_malloc(sizeof(struct k_sound));
    if (NULL == sound)
        return NULL;

    Mix_Music *music = Mix_LoadMUS(filepath);
    if (NULL == music) {
        k_free(sound);
        return NULL;
    }

    sound->sound = music;
    return sound;
}
