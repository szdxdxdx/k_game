#include "../asset/k_asset_registry.h"

#include "./k_component_type.h"
#include "./k_component_type_registry.h"

struct k_asset_registry k__component_type_registry;

int k__component_type_registry_init(void) {
    return k__asset_registry_init(&k__component_type_registry);
}

static void release_asset(struct k_asset_registry_node *registry_node) {
    struct k_component_type *component_type = container_of(registry_node, struct k_component_type, registry_node);
    k__component_undef(component_type);
}

void k__component_type_registry_cleanup(void) {
    k__asset_registry_cleanup(&k__component_type_registry, release_asset);
}

int k_component_type_set_name(struct k_component_type *component_type, const char *type_name) {
    return k__asset_set_name(&k__component_type_registry, &component_type->registry_node, type_name);
}

struct k_component_type *k_component_type_find(const char *type_name) {

    struct k_asset_registry_node *registry_node = k__asset_registry_find(&k__component_type_registry, type_name);
    if (NULL == registry_node)
        return NULL;

    struct k_component_type *component_type = container_of(registry_node, struct k_component_type, registry_node);
    return component_type;
}
