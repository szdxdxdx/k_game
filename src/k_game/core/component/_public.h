#ifndef K_GAME__COMPONENT__PUBLIC_H
#define K_GAME__COMPONENT__PUBLIC_H

#include <stddef.h>

#include "../asset/k_asset_registry.h"

struct k_object;
struct k_component_type;
struct k_component_manager;
struct k_component;

/* region [component_type_registry] */

int k__component_type_registry_init(void);

void k__component_type_registry_cleanup(void);

/* endregion */

/* region [component_type] */

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

/* endregion */

/* region [component] */

struct k_component {

    struct k_list_node list_node;

    struct k_component_type *type;

    struct k_object *object;

    struct k_component_manager *manager;

    struct k_list callback_list;

    void *data;
};

/* endregion */

#endif
