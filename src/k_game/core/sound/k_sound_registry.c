#include "k_game/core/k_sound.h"

#include "./k_sound_bgm.h"
#include "./k_sound_sfx.h"
#include "./k_sound_registry.h"

/* region [sound_bgm_registry] */

static struct k_asset_registry bgm_registry;

int k__sound_bgm_registry_init(void) {
    return k__asset_registry_init(&bgm_registry);
}

static void release_bgm_asset(struct k_asset_registry_node *registry_node) {
    struct k_sound_bgm *sound = container_of(registry_node, struct k_sound_bgm, registry_node);
    k_sound_bgm_release(sound);
}

void k__sound_bgm_registry_cleanup(void) {
    k__asset_registry_cleanup(&bgm_registry, release_bgm_asset);
}

struct k_sound_bgm *k_sound_bgm_find(const char *bgm_name) {
    struct k_asset_registry_node *registry_node = k__asset_registry_find(&bgm_registry, bgm_name);
    if (NULL == registry_node)
        return NULL;

    struct k_sound_bgm *sound = container_of(registry_node, struct k_sound_bgm, registry_node);
    return sound;
}

/* endregion */

/* region [sound_sfx_registry] */

static struct k_asset_registry sfx_registry;

int k__sound_sfx_registry_init(void) {
    return k__asset_registry_init(&sfx_registry);
}

static void release_sfx_asset(struct k_asset_registry_node *registry_node) {
    struct k_sound_sfx *sound = container_of(registry_node, struct k_sound_sfx, registry_node);
    k_sound_sfx_release(sound);
}

void k__sound_sfx_registry_cleanup(void) {
    k__asset_registry_cleanup(&sfx_registry, release_sfx_asset);
}

struct k_sound_sfx *k_sound_sfx_find(const char *sfx_name) {
    struct k_asset_registry_node *registry_node = k__asset_registry_find(&sfx_registry, sfx_name);
    if (NULL == registry_node)
        return NULL;

    struct k_sound_sfx *sound = container_of(registry_node, struct k_sound_sfx, registry_node);
    return sound;
}

/* endregion */
