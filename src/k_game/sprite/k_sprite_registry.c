#include "./k_sprite.h"

static struct k_asset_registry sprite_registry;

static void fn_free_asset(struct k_asset_registry_node *registry_node) {
    struct k_sprite *sprite = container_of(registry_node, struct k_sprite, registry_node);
    k__sprite_destroy(sprite);
}

int k__sprite_registry_init(void) {
    return k__asset_registry_init(&sprite_registry, fn_free_asset);
}

void k__sprite_registry_cleanup(void) {
     k__asset_registry_cleanup(&sprite_registry);
}

int k__sprite_registry_add(struct k_sprite *sprite, const char *sprite_name) {
    return k__asset_registry_add(&sprite_registry, &sprite->registry_node, sprite_name);
}

void k__sprite_registry_del(struct k_sprite *sprite) {
    k__asset_registry_del(&sprite->registry_node);
}

struct k_sprite *k_sprite_find(const char *sprite_name) {

    struct k_asset_registry_node *registry_node = k__asset_registry_find(&sprite_registry, sprite_name);
    if (NULL == registry_node)
        return NULL;

    struct k_sprite *sprite = container_of(registry_node, struct k_sprite, registry_node);
    return sprite;
}

const char *k__sprite_get_name(struct k_sprite *sprite) {
    return k__asset_get_name(&sprite->registry_node);
}
