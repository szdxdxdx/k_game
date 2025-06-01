
#define K_LOG_TAG "k_game:sound"
#include "k_log.h"

#include "k_game/core/k_mem_alloc.h"
#include "k_game/core/k_sound.h"

#include "./k_sound.h"
#include "./k_sound_registry.h"

/* region [bgm] */

struct k_sound_bgm *k_sound_bgm_load(const char *file_path) {

    if (NULL == file_path || '\0' == file_path[0])
        return NULL;

    struct k_sound_bgm *sound = k__mem_alloc(sizeof(struct k_sound_bgm));
    if (NULL == sound)
        return NULL;

    Mix_Music *music = Mix_LoadMUS(file_path);
    if (NULL == music) {
        k_log_error("SDL error: %s", Mix_GetError());
        k__mem_free(sound);
        return NULL;
    }

    sound->music = music;
    k__sound_bgm_registry_add(sound);
    return sound;
}

void k_sound_bgm_release(struct k_sound_bgm *sound) {

    if (NULL == sound)
        return;

    k__sound_bgm_registry_del(sound);
    Mix_FreeMusic(sound->music);
    k__mem_free(sound);
}

/* endregion */

/* region [sfx] */

struct k_sound_sfx *k_sound_sfx_load(const char *file_path) {

    if (NULL == file_path || '\0' == file_path[0])
        return NULL;

    struct k_sound_sfx *sound = k__mem_alloc(sizeof(struct k_sound_sfx));
    if (NULL == sound)
        return NULL;

    Mix_Chunk *chunk = Mix_LoadWAV(file_path);
    if (NULL == chunk) {
        k__mem_free(sound);
        return NULL;
    }

    sound->chunk = chunk;
    k__sound_sfx_registry_add(sound);
    return sound;
}

void k_sound_sfx_release(struct k_sound_sfx *sound) {

    if (NULL == sound)
        return;

    k__sound_sfx_registry_del(sound);
    Mix_FreeChunk(sound->chunk);
    k__mem_free(sound);
}

/* endregion */
