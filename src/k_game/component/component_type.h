#ifndef K_GAME__COMPONENT_TYPE_H
#define K_GAME__COMPONENT_TYPE_H

#include "../asset/asset_registry.h"

struct k_component;

struct k_component_type {

    struct k_asset_registry_node registry_node;

    size_t data_size;

    int (*fn_init)(struct k_component *component, void *params);

    void (*fn_fini)(struct k_component *component);
};

void k__component_undef(struct k_component_type *component_type);

const char *k__component_type_get_name(struct k_component_type *component_type);

#endif
