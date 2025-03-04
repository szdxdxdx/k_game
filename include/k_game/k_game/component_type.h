#ifndef K_GAME__COMPONENT_TYPE_H
#define K_GAME__COMPONENT_TYPE_H

#include "k_game/asset_registry.h"

struct k_component;
struct k_component_type;

/* region [component_registry] */

int k__component_registry_init(void);

void k__component_registry_cleanup(void);

int k__component_registry_add(struct k_component_type *component_type, const char *type_name);

void k__component_registry_del(struct k_component_type *component_type);

/* endregion */

/* region [component_type] */

struct k_component_type {

    struct k_asset_registry_node registry_node;

    size_t data_size;

    int (*fn_init)(struct k_component *component, void *params);

    void (*fn_fini)(struct k_component *component);
};

void k__component_undef(struct k_component_type *component_type);

const char *k__component_type_get_name(struct k_component_type *component_type);

/* endregion */

/* region [k_game component] */

int k__component_def_WASD(void);
int k__component_def_sprite_renderer(void);

/* endregion */

#endif
