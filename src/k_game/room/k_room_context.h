#ifndef K_GAME__ROOM_CONTEXT_H
#define K_GAME__ROOM_CONTEXT_H

#include <stdint.h>

#include "../object/k_object_pool.h"
#include "./k_room_registry.h"
#include "./k_room_callback_alarm.h"
#include "./k_room_callback_draw.h"

struct k_room {

    struct k_room_registry_node room_node;

    struct k_room_callback_list enter_callbacks;

    struct k_room_callback_list leave_callbacks;

    struct k_room_callback_list step_begin_callbacks;

    struct k_room_callback_list step_callbacks;

    struct k_room_callback_list step_end_callbacks;

    struct k_room_alarm_callbacks_storage alarm_callbacks;

    struct k_room_draw_callbacks_storage draw_callbacks;

    struct k_object_pool object_pool;

    int room_w, room_h;

    int (*fn_create)(struct k_room *room, void *params);

    void (*fn_destroy)(struct k_room *room);

    uint64_t step_interval_ms;

    unsigned int game_loop;

    void *data;
};

void k__room_run(struct k_room *room);

#endif
