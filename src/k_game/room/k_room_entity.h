#ifndef K_GAME__ROOM_ENTITY_H
#define K_GAME__ROOM_ENTITY_H

#include <stdint.h>

#include "k_list.h"

#include "../asset/k_asset_registry.h"
#include "../callback/k_callback.h"
#include "../object/k_object_pool.h"

struct k_object_pool;

struct k_room {

    struct k_asset_registry_node registry_node;

    struct k_alarm_callback_manager alarm_callback_manager;

    struct k_step_callback_manager step_callback_manager;

    struct k_draw_callback_manager draw_callback_manager;

    struct k_list callback_list;

    struct k_object_pool object_pool;

    int room_w, room_h;

    int (*fn_init)(struct k_room *room, void *params);

    void (*fn_cleanup)(struct k_room *room);

    uint64_t step_interval_ms;

    unsigned int game_loop;

    void *data;
};

#endif
