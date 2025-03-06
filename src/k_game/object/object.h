#ifndef K_GAME__OBJECT_H
#define K_GAME__OBJECT_H

#include "k_list.h"

#include "k_game_object.h"
#include "./object_pool.h"

/* region [object] */

struct k_object {

    struct k_object_pool_node pool_node;

    struct k_room *room;

    struct k_list callback_list;

    struct k_list component_list;

    void *data;
};

void k__object_destroy(struct k_object *object);

/* endregion */

/* region [object_callback] */

struct k_room_callback;

struct k_object_callback {

    struct k_list_node object_callback_list_node;

    struct k_room_callback *room_callback;

    struct k_object *object;

    union {
        void (*fn_step_begin_callback)(struct k_object *object);
        void (*fn_alarm_callback)     (struct k_object *object, int timeout_diff);
        void (*fn_step_callback)      (struct k_object *object);
        void (*fn_draw_callback)      (struct k_object *object);
        void (*fn_step_end_callback)  (struct k_object *object);
    };
};

void k__object_init_callback_list(struct k_object *object);

void k__object_cleanup_callback_list(struct k_object *object);

/* endregion */

/* region [object_component] */

struct k_component;

void k__object_init_component_list(struct k_object *object);

void k__object_cleanup_component_list(struct k_object *object);

void k__object_component_list_add(struct k_object *object, struct k_component *component);

void k__object_component_list_del(struct k_component *component);

/* endregion */

#endif
