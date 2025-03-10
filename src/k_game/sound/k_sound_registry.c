#include "./k_sound.h"

static struct k_asset_registry sound_registry;

static void fn_free_asset(struct k_asset_registry_node *node) {
    struct k_sound *sound = container_of(node, struct k_sound, registry_node);
    k__sound_release(sound);
}

int k__sound_registry_init(void) {
    return k__asset_registry_init(&sound_registry, fn_free_asset);
}

void k__sound_registry_cleanup(void) {
    return k__asset_registry_cleanup(&sound_registry);
}

void k__sound_registry_add(struct k_sound *sound) {
    return k__asset_registry_add(&sound_registry, &sound->registry_node);
}

void k__sound_registry_del(struct k_sound *sound) {
    return k__asset_registry_del(&sound->registry_node);
}

int k_sound_set_name(struct k_sound *sound, const char *name) {
    return k__asset_set_name(&sound_registry, &sound->registry_node, name);
}

struct k_sound *k_sound_find(const char *sound_name) {

    struct k_asset_registry_node *registry_node = k__asset_registry_find(&sound_registry, sound_name);
    if (NULL == registry_node)
        return NULL;

    struct k_sound *sound = container_of(registry_node, struct k_sound, registry_node);
    return sound;
}
