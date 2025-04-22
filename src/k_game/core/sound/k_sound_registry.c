#include "k_game/core/k_sound.h"

#include "./k_sound_bgm.h"
#include "./k_sound_sfx.h"
#include "./k_sound_registry.h"

/* region [sound_bgm_registry] */

static struct k_asset_registry bgm_registry;

int k__sound_bgm_registry_init(void) {
    return k__asset_registry_init(&bgm_registry);
}

static void k__sound_bgm_registry_release(struct k_asset_registry_node *registry_node) {
    struct k_sound_bgm *sound = container_of(registry_node, struct k_sound_bgm, registry_node);
    k_sound_bgm_release(sound);
}

void k__sound_bgm_registry_cleanup(void) {
    k__asset_registry_cleanup(&bgm_registry, k__sound_bgm_registry_release);
}

void k__sound_bgm_registry_add(struct k_sound_bgm *bgm) {
    k__asset_registry_add(&bgm_registry, &bgm->registry_node);
}

void k__sound_bgm_registry_del(struct k_sound_bgm *bgm) {
    k__asset_registry_del(&bgm->registry_node);
}

int k_sound_bgm_set_name(struct k_sound_bgm *sound, const char *bgm_name) {
    return k__asset_set_name(&bgm_registry, &sound->registry_node, bgm_name);
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

static void k__sound_sfx_registry_release(struct k_asset_registry_node *registry_node) {
    struct k_sound_sfx *sound = container_of(registry_node, struct k_sound_sfx, registry_node);
    k_sound_sfx_release(sound);
}

void k__sound_sfx_registry_cleanup(void) {
    k__asset_registry_cleanup(&sfx_registry, k__sound_sfx_registry_release);
}

void k__sound_sfx_registry_add(struct k_sound_sfx *sfx) {
    k__asset_registry_add(&sfx_registry, &sfx->registry_node);
}

void k__sound_sfx_registry_del(struct k_sound_sfx *sfx) {
    k__asset_registry_del(&sfx->registry_node);
}

int k_sound_sfx_set_name(struct k_sound_sfx *sound, const char *sfx_name) {
    return k__asset_set_name(&sfx_registry, &sound->registry_node, sfx_name);
}

struct k_sound_sfx *k_sound_sfx_find(const char *sfx_name) {
    struct k_asset_registry_node *registry_node = k__asset_registry_find(&sfx_registry, sfx_name);
    if (NULL == registry_node)
        return NULL;

    struct k_sound_sfx *sound = container_of(registry_node, struct k_sound_sfx, registry_node);
    return sound;
}

/* endregion */
