#include "k_log.h"

#include "_internal.h"

/* region [sound_registry] */

static struct k_asset_registry bgm_registry;

int k__init_sound_bgm_registry(void) {
    return k__asset_registry_init(&bgm_registry);
}

static void release_asset(struct k_asset_registry_node *registry_node) {
    struct k_sound_bgm *sound = container_of(registry_node, struct k_sound_bgm, registry_node);
    k_sound_bgm_release(sound);
}

void k__cleanup_sound_bgm_registry(void) {
    k__asset_registry_cleanup(&bgm_registry, release_asset);
}

struct k_sound_bgm *k_sound_bgm_find(const char *bgm_name) {
    struct k_asset_registry_node *registry_node = k__asset_registry_find(&bgm_registry, bgm_name);
    if (NULL == registry_node)
        return NULL;

    struct k_sound_bgm *sound = container_of(registry_node, struct k_sound_bgm, registry_node);
    return sound;
}

/* endregion */

/* region [sound_load] */

struct k_sound_bgm *k_sound_bgm_load(const char *filepath) {

    if (NULL == filepath || '\0' == filepath[0])
        return NULL;

    struct k_sound_bgm *sound = k_malloc(sizeof(struct k_sound_bgm));
    if (NULL == sound)
        return NULL;

    Mix_Music *music = Mix_LoadMUS(filepath);
    if (NULL == music) {
        k_log_error("Mix_LoadMUS(): %s", Mix_GetError());
        k_free(sound);
        return NULL;
    }

    sound->music = music;
    k__asset_registry_add(&bgm_registry, &sound->registry_node);
    return sound;
}

void k_sound_bgm_release(struct k_sound_bgm *sound) {

    if (NULL == sound)
        return;

    k__asset_registry_del(&sound->registry_node);
    Mix_FreeMusic(sound->music);
    k_free(sound);
}

/* endregion */

/* region [sound_play] */

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

/* endregion */
