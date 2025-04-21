#include "k_game/core/k_component.h"
#include "./k_component_type.h"
#include "./k_component_type_registry.h"

static struct k_asset_registry component_type_registry;

int k__component_type_registry_init(void) {
    return k__asset_registry_init(&component_type_registry);
}

static void release_asset(struct k_asset_registry_node *registry_node) {
    struct k_component_type *component_type = container_of(registry_node, struct k_component_type, registry_node);
    k__component_type_unregister(component_type);
}

void k__component_type_registry_cleanup(void) {
    k__asset_registry_cleanup(&component_type_registry, release_asset);
}

void k__component_type_registry_add(struct k_component_type *component_type) {
    k__asset_registry_add(&component_type_registry, &component_type->registry_node);
}

void k__component_type_registry_del(struct k_component_type *component_type) {
    k__asset_registry_del(&component_type->registry_node);
}

int k_component_type_set_name(struct k_component_type *component_type, const char *type_name) {
    return k__asset_set_name(&component_type_registry, &component_type->registry_node, type_name);
}

struct k_component_type *k_component_type_find(const char *type_name) {

    struct k_asset_registry_node *registry_node = k__asset_registry_find(&component_type_registry, type_name);
    if (NULL == registry_node)
        return NULL;

    struct k_component_type *component_type = container_of(registry_node, struct k_component_type, registry_node);
    return component_type;
}
