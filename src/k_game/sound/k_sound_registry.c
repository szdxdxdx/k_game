#include "./k_sound.h"

static struct k_asset_registry BGM_registry, SFX_registry;

int k__sound_registry_init(void) {

    if (0 != k__asset_registry_init(&BGM_registry))
        return -1;

    if (0 != k__asset_registry_init(&SFX_registry)) {
        k__asset_registry_cleanup(&BGM_registry, NULL);
        return -1;
    }

    return 0;
}

static void fn_release_BGM(struct k_asset_registry_node *registry_node) {
    struct k_sound_BGM *sound = (struct k_sound_BGM *)registry_node;
    k_sound_BGM_release(sound);
}

static void fn_release_SFX(struct k_asset_registry_node *registry_node) {
    struct k_sound_SFX *sound = (struct k_sound_SFX *)registry_node;
    k_sound_SFX_release(sound);
}

void k__sound_registry_cleanup(void) {
    k__asset_registry_cleanup(&BGM_registry, fn_release_BGM);
    k__asset_registry_cleanup(&SFX_registry, fn_release_SFX);
}

void k__sound_registry_add_BGM(struct k_sound_BGM *sound) {
    k__asset_registry_add(&BGM_registry, &sound->registry_node);
}

void k__sound_registry_add_SFX(struct k_sound_SFX *sound) {
    k__asset_registry_add(&SFX_registry, &sound->registry_node);
}

void k__sound_registry_del_BGM(struct k_sound_BGM *sound) {
    k__asset_registry_del(&sound->registry_node);
}

void k__sound_registry_del_SFX(struct k_sound_SFX *sound) {
    k__asset_registry_del(&sound->registry_node);
}

int k_sound_BGM_set_name(struct k_sound_BGM *sound, const char *name) {
    return k__asset_set_name(&BGM_registry, &sound->registry_node, name);
}

int k_sound_SFX_set_name(struct k_sound_SFX *sound, const char *name) {
    return k__asset_set_name(&SFX_registry, &sound->registry_node, name);
}

struct k_sound_BGM *k_sound_BGM_find(const char *BGM_name) {
    struct k_asset_registry_node *registry_node = k__asset_registry_find(&BGM_registry, BGM_name);
    if (NULL == registry_node)
        return NULL;

    struct k_sound_BGM *sound = container_of(registry_node, struct k_sound_BGM, registry_node);
    return sound;
}

struct k_sound_SFX *k_sound_SFX_find(const char *SFX_name) {
    struct k_asset_registry_node *registry_node = k__asset_registry_find(&SFX_registry, SFX_name);
    if (NULL == registry_node)
        return NULL;

    struct k_sound_SFX *sound = container_of(registry_node, struct k_sound_SFX, registry_node);
    return sound;
}
