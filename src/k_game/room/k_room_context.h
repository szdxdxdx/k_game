#ifndef K_GAME__ROOM_CONTEXT_H
#define K_GAME__ROOM_CONTEXT_H

#include <stdint.h>

#include "./k_room_registry.h"

struct k_room {

    struct k_room_registry_node room_node;

    int (*fn_create)(struct k_room *room, void *params);

    void (*fn_destroy)(struct k_room *room);

    uint32_t frame_interval;

    unsigned int game_loop;

    uint64_t current_time;

    int delta_ms;

    void *data;
};

#endif
