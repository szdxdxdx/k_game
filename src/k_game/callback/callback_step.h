#ifndef K_GAME__CALLBACK_STEP_H
#define K_GAME__CALLBACK_STEP_H

#include "k_list.h"

#include "./callback_base.h"

#include "../room/room_callback.h"
#include "../object/object_callback.h"
#include "../component/component_callback.h"

struct k_step_callback {

    struct k_list_node list_node;

    struct k_callback base;
};

struct k_step_callback_manager {

    struct k_list callback_list;

    struct k_list pending_list;
};

void k__init_step_callback_manager(struct k_step_callback_manager *manager);

void k__deinit_step_callback_manager(struct k_step_callback_manager *manager);

void k__flush_step_callbacks(struct k_step_callback_manager *manager);

void k__exec_step_callbacks(struct k_step_callback_manager *manager);

/* region [room_step_callback] */

struct k_room_step_callback {

    struct k_step_callback step_callback;

    struct k_room_callback room_callback;

    void (*fn_callback)(void *data);

    void *data;
};

struct k_room_callback *k__add_room_step_callback(struct k_step_callback_manager *manager, void (*fn_callback)(void *data), void *data);

/* endregion */

/* region [object_step_callback] */

struct k_object_step_callback {

    struct k_step_callback step_callback;

    struct k_object_callback object_callback;

    void (*fn_callback)(struct k_object *object);

    struct k_object *object;
};

struct k_object_callback *k__add_object_step_callback(struct k_step_callback_manager *manager, void (*fn_callback)(struct k_object *object), struct k_object *object);

/* endregion */

/* region [component_step] */

struct k_component_step_callback {

    struct k_step_callback step_callback;

    struct k_component_callback component_callback;

    void (*fn_callback)(struct k_component *component);

    struct k_component *component;
};

struct k_component_callback *k__add_component_step_callback(struct k_step_callback_manager *manager, void (*fn_callback)(struct k_component *component), struct k_component *component);

/* endregion */

#endif
