#ifndef K_GAME__CALLBACK_ALARM_H
#define K_GAME__CALLBACK_ALARM_H

#include <stdint.h>

#include "k_list.h"

#include "./callback_base.h"

#include "../room/room_callback.h"
#include "../object/object_callback.h"
#include "../component/component_callback.h"

struct k_alarm_callback {

    struct k_list_node list_node;

    struct k_callback base;

    uint64_t timeout;
};

struct k_alarm_callback_manager {

    struct k_list callback_list;

    struct k_list pending_list;
};

void k__init_alarm_callback_manager(struct k_alarm_callback_manager *manager);

void k__deinit_alarm_callback_manager(struct k_alarm_callback_manager *manager);

void k__flush_alarm_callbacks(struct k_alarm_callback_manager *manager);

void k__exec_alarm_callbacks(struct k_alarm_callback_manager *manager);

/* region [room_alarm_callback] */

struct k_room_alarm_callback {

    struct k_alarm_callback alarm_callback;

    struct k_room_callback room_callback;

    void (*fn_callback)(void *data,  int timeout_diff);

    void *data;
};

struct k_room_callback *k__add_room_alarm_callback(struct k_alarm_callback_manager *manager, void (*fn_callback)(void *data,  int timeout_diff), void *data, int delay_ms);

/* endregion */

/* region [object_alarm_callback] */

struct k_object_alarm_callback {

    struct k_alarm_callback alarm_callback;

    struct k_object_callback object_callback;

    void (*fn_callback)(struct k_object *object, int timeout_diff);

    struct k_object *object;
};

struct k_object_callback *k__add_object_alarm_callback(struct k_alarm_callback_manager *manager, void (*fn_callback)(struct k_object *object, int timeout_diff), struct k_object *object, int delay_ms);

/* endregion */

/* region [object_alarm_callback] */

struct k_component_alarm_callback {

    struct k_alarm_callback alarm_callback;

    struct k_component_callback component_callback;

    void (*fn_callback)(struct k_component *component, int timeout_diff);

    struct k_component *component;
};

struct k_component_callback *k__add_component_alarm_callback(struct k_alarm_callback_manager *manager, void (*fn_callback)(struct k_component *component, int timeout_diff), struct k_component *component, int delay_ms);

/* endregion */

#endif
