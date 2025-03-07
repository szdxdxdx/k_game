#ifndef K_GAME__CALLBACK_ALARM_H
#define K_GAME__CALLBACK_ALARM_H

#include <stdint.h>

#include "k_list.h"

#include "./callback_base.h"

#include "../room/room_callback.h"
#include "../object/object_callback.h"

struct k_alarm_callback {

    struct k_list_node callback_list_node;

    uint64_t timeout;
};

struct k_alarm_callback_manager {

    struct k_list callback_list;
};

void k__init_alarm_callback_manager(struct k_alarm_callback_manager *manager);

void k__deinit_alarm_callback_manager(struct k_alarm_callback_manager *manager);

void k__exec_alarm_callbacks(struct k_alarm_callback_manager *manager);

void k__flush_alarm_callbacks(struct k_alarm_callback_manager *manager);

/* region [room_alarm_callback] */

struct k_room_alarm_callback {

    struct k_alarm_callback alarm_callback;

    struct k_room_callback room_callback;

    void (*fn_callback)(void *data,  int timeout_diff);

    void *data;
};

/* endregion */

/* region [object_alarm_callback] */

struct k_object_alarm_callback {

    struct k_alarm_callback alarm_callback;

    struct k_object_callback object_callback;

    void (*fn_callback)(struct k_object *object, int timeout_diff);

    struct k_object *object;
};

/* endregion */

#endif
