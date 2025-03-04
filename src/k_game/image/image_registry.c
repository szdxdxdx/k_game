#include "k_game/image_asset.h"

static struct k_asset_registry image_registry;

static void fn_free_asset(struct k_asset_registry_node *node) {
    struct k_image *image = container_of(node, struct k_image, registry_node);
    k__image_release(image);
}

int k__image_registry_init(void) {
    return k__asset_registry_init(&image_registry, fn_free_asset);
}

void k__image_registry_cleanup(void) {
     k__asset_registry_cleanup(&image_registry);
}

int k__image_registry_add(struct k_image *image, const char *image_name) {
    return k__asset_registry_add(&image_registry, &image->registry_node, image_name);
}

void k__image_registry_del(struct k_image *image) {
    k__asset_registry_del(&image->registry_node);
}

struct k_image *k_image_find(const char *image_name) {
    struct k_asset_registry_node *registry_node = k__asset_registry_get(&image_registry, image_name);
    if (NULL == registry_node)
        return NULL;

    struct k_image *image = container_of(registry_node, struct k_image, registry_node);
    return image;
}

const char *k__image_get_name(struct k_image *image) {
    return k__asset_get_name(&image->registry_node);
}
