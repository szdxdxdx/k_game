#ifndef K_GAME__COMPONENT_H
#define K_GAME__COMPONENT_H

#include "k_list.h"

#include "k_game_component.h"
#include "../asset/asset_registry.h"

struct k_component_type;
struct k_component;

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

/* region [component] */

struct k_component {

    struct k_list_node object_component_list_node;

    const struct k_component_type *type;

    struct k_object *object;

    struct k_list callback_list;

    void *data;
};

struct k_component *k__component_create(const struct k_component_type *component_type, struct k_object *object, void *params);

void k__component_destroy(struct k_component *component);

/* endregion */

/* region [component_callback] */

struct k_component_callback {

    struct k_list_node component_callback_list_node;

    struct k_room_callback *room_callback;

    struct k_component *component;

    union {
        void (*fn_alarm_callback)(struct k_component *component, int timeout_diff);
        void (*fn_step_callback) (struct k_component *component);
        void (*fn_draw_callback) (struct k_component *component);
    };
};

void k__component_init_callback_list(struct k_component *component);

void k__component_cleanup_callback_list(struct k_component *component);

/* endregion */

#endif
