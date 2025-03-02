#ifndef K_GAME__COMPONENT_TYPE_H
#define K_GAME__COMPONENT_TYPE_H

#include "k_game/asset_registry.h"

struct k_component;
struct k_component_type;

/* region [component_type_registry] */

int k__component_registry_init(void);

void k__component_registry_cleanup(void);

int k__component_registry_add(struct k_component_type *component_type, const char *type_name);

void k__component_registry_del(struct k_component_type *component_type);

/* endregion */

/* region [component_type] */

struct k_component_type {

    struct k_asset_registry_node registry_node;

    size_t data_size;

    int (*fn_create)(struct k_component *component, void *params);

    void (*fn_destroy)(struct k_component *component);
};

void k__undef_component_type(struct k_component_type *component_type);

/* endregion */

#endif
