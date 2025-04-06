#ifndef K_COMPONENT_TYPE_H
#define K_COMPONENT_TYPE_H

#include "../asset/k_asset_registry.h"

struct k_component;
struct k_component_manager;

struct k_component_entity_type {

    size_t data_size;

    int (*fn_init)(struct k_component *component, void *params);

    void (*fn_fini)(struct k_component *component);
};

struct k_component_manager_type {

    size_t type_id;

    size_t data_size;

    int (*fn_init)(struct k_component_manager *manager, void *params);

    void (*fn_fini)(struct k_component_manager *manager);
};

struct k_component_type {

    struct k_asset_registry_node registry_node;

    struct k_component_entity_type entity_type;

    struct k_component_manager_type *manager_type;
};

void k__component_undef(struct k_component_type *component_type);

#endif
