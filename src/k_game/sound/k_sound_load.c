#include "k_game_alloc.h"

#include "./k_sound.h"

static struct k_sound *k_sound_load_BGM(const char *filepath) {

    struct k_sound *sound = k_malloc(sizeof(struct k_sound));
    if (NULL == sound)
        return NULL;

    Mix_Music *music = Mix_LoadMUS(filepath);
    if (NULL == music) {
        k_free(sound);
        return NULL;
    }

    sound->sound_type = K_SOUND_BGM;
    sound->music = music;
    return sound;
}

static struct k_sound *k_sound_load_SFX(const char *filepath) {

    struct k_sound *sound = k_malloc(sizeof(struct k_sound));
    if (NULL == sound)
        return NULL;

    Mix_Chunk *chunk = Mix_LoadWAV(filepath);
    if (NULL == chunk) {
        k_free(sound);
        return NULL;
    }

    sound->sound_type = K_SOUND_SFX;
    sound->chunk = chunk;
    return sound;
}

struct k_sound *k_sound_load(const char *filepath, enum k_sound_type sound_type) {

    if (NULL == filepath || '\0' == filepath[0]) {
        /* TODO log error */
        return NULL;
    }

    switch (sound_type) {
        case K_SOUND_BGM:
            return k_sound_load_BGM(filepath);
        case K_SOUND_SFX:
            return k_sound_load_SFX(filepath);
        default:
            return NULL;
    }
}
