#ifndef K_GAME__ROOM_CONTEXT_H
#define K_GAME__ROOM_CONTEXT_H

#include <stdint.h>

#include "../asset/asset_registry.h"
#include "../callback/callback_step.h"
#include "../object/object_pool.h"

struct k_object_pool;

struct k_room {

    struct k_asset_registry_node registry_node;

    struct k_step_callback_manager step_callback_manager;

    struct k_object_pool object_pool;

    int room_w, room_h;

    int (*fn_init)(struct k_room *room, void *params);

    void (*fn_cleanup)(struct k_room *room);

    uint64_t step_interval_ms;

    unsigned int game_loop;

    void *data;
};

#endif
