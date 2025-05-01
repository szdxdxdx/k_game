#ifndef K__ROOM_ENTITY_H
#define K__ROOM_ENTITY_H

#include <stddef.h>

#include "k_list.h"

#include "../asset/k_asset_registry.h"
#include "../object/k_object_pool.h"
#include "../callback/k_callback_alarm.h"
#include "../callback/k_callback_step.h"
#include "../callback/k_callback_draw.h"

struct k_room {

    struct k_asset_registry_node registry_node;

    size_t room_id;

    struct k_step_callback_manager  step_begin_callback_manager;
    struct k_alarm_callback_manager alarm_callback_manager;
    struct k_step_callback_manager  step_callback_manager;
    struct k_draw_callback_manager  draw_callback_manager;
    struct k_step_callback_manager  step_end_callback_manager;

    struct k_list callback_list;

    int  (*on_create)(void *param);
    void (*on_destroy)(void);

    void (*on_enter)(void);
    void (*on_leave)(void);

    struct k_object_pool object_pool;

    float room_w;
    float room_h;

    uint64_t step_interval_ms;

    unsigned int game_loop;

    void *data;
};

extern struct k_room *k__current_room;

#endif
