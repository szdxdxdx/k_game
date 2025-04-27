
#include "k_game/core/k_mem_alloc.h"

#include "k_game/core/k_font.h"
#include "./k_font.h"
#include "./k_font_registry.h"

#include "k_font_builtin_ttf.h"

static struct k_asset_registry font_registry;

int k__font_registry_init(void) {
    return k__asset_registry_init(&font_registry);
}

static void k__font_registry_release(struct k_asset_registry_node *node) {
    struct k_font *font = container_of(node, struct k_font, registry_node);
    k_font_release(font);
}

void k__font_registry_cleanup(void) {
    k__asset_registry_cleanup(&font_registry, k__font_registry_release);
}

void k__font_registry_add(struct k_font *font) {
    k__asset_registry_add(&font_registry, &font->registry_node);
}

void k__font_registry_del(struct k_font *font) {
    k__asset_registry_del(&font->registry_node);
}

int k_font_set_name(struct k_font *font, const char *font_name) {
    return k__asset_set_name(&font_registry, &font->registry_node, font_name);
}

struct k_font *k_font_find(const char *font_name) {
    struct k_asset_registry_node *registry_node = k__asset_registry_find(&font_registry, font_name);
    if (NULL == registry_node)
        return NULL;

    struct k_font *font = container_of(registry_node, struct k_font, registry_node);
    return font;
}
