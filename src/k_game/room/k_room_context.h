#ifndef K_GAME__ROOM_CONTEXT_H
#define K_GAME__ROOM_CONTEXT_H

#include <stdint.h>

#include "./k_room_registry.h"
#include "./k_room_callback.h"

struct k_room {

    struct k_room_registry_node room_node;

    int (*fn_create)(struct k_room *room, void *params);

    void (*fn_destroy)(struct k_room *room);

    uint32_t frame_interval;

    unsigned int game_loop;

    void *data;

    struct k_room_alarm_callbacks_storage alarm_callbacks;

    struct k_room_step_callbacks_storage step_callbacks;

    struct k_room_draw_callbacks_storage draw_callbacks;
};

#endif
