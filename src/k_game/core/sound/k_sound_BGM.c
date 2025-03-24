#include "k_log.h"

#include "_internal.h"

/* region [sound_registry] */

static struct k_asset_registry BGM_registry;

int k__sound_BGM_registry_init(void) {
    return k__asset_registry_init(&BGM_registry);
}

static void fn_release_BGM(struct k_asset_registry_node *registry_node) {
    struct k_sound_BGM *sound = (struct k_sound_BGM *)registry_node;
    k_release_sound_BGM(sound);
}

void k__sound_BGM_registry_cleanup(void) {
    k__asset_registry_cleanup(&BGM_registry, fn_release_BGM);
}

struct k_sound_BGM *k_find_sound_BGM(const char *BGM_name) {
    struct k_asset_registry_node *registry_node = k__asset_registry_find(&BGM_registry, BGM_name);
    if (NULL == registry_node)
        return NULL;

    struct k_sound_BGM *sound = container_of(registry_node, struct k_sound_BGM, registry_node);
    return sound;
}

/* endregion */

/* region [sound_load] */

struct k_sound_BGM *k_load_sound_BGM(const char *filepath) {

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
    k__asset_registry_add(&BGM_registry, &sound->registry_node);
    return sound;
}

void k_release_sound_BGM(struct k_sound_BGM *sound) {

    if (NULL == sound)
        return;

    k__asset_registry_del(&sound->registry_node);
    Mix_FreeMusic(sound->music);
    k_free(sound);
}

/* endregion */

/* region [sound_play] */

int k_loop_sound_BGM(struct k_sound_BGM *sound, int loops) {

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
