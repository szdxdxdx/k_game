#ifndef K_GAME__COMPONENT_ENTITY_H
#define K_GAME__COMPONENT_ENTITY_H

#include "k_list.h"

struct k_room_callback;
struct k_object;
struct k_component_type;
struct k_component;

/* region [component] */

struct k_component {

    struct k_list_node object_component_list_node;

    const struct k_component_type *type;

    struct k_object *object;

    struct k_list callbacks;

    void *data;
};

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

/* region [object_component] */

void k__object_init_component_list(struct k_object *object);

void k__object_cleanup_component_list(struct k_object *object);

/* endregion */

#endif
