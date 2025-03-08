#include "./k_component.h"

static struct k_asset_registry component_registry;

static void fn_free_asset(struct k_asset_registry_node *registry_node) {
    struct k_component_type *component_type = container_of(registry_node, struct k_component_type, registry_node);
    k__component_undef(component_type);
}

int k__component_registry_init(void) {
    return k__asset_registry_init(&component_registry, fn_free_asset);
}

void k__component_registry_cleanup(void) {
    k__asset_registry_cleanup(&component_registry);
}

int k__component_registry_add(struct k_component_type *component_type, const char *type_name) {
    return k__asset_registry_add(&component_registry, &component_type->registry_node, type_name);
}

void k__component_registry_del(struct k_component_type *component_type) {
    k__asset_registry_del(&component_type->registry_node);
}

struct k_component_type *k_component_type_find(const char *type_name) {

    struct k_asset_registry_node *registry_node = k__asset_registry_find(&component_registry, type_name);
    if (NULL == registry_node)
        return NULL;

    struct k_component_type *component_type = container_of(registry_node, struct k_component_type, registry_node);
    return component_type;
}

const char *k__component_type_get_name(struct k_component_type *component_type) {
    return k__asset_get_name(&component_type->registry_node);
}
