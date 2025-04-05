#ifndef K_ROOM_SHARED_H
#define K_ROOM_SHARED_H

#include <stddef.h>
#include <stdint.h>

#include "k_list.h"

#include "../asset/k_asset_registry.h"
#include "../callback/_shared.h"
#include "../object/_shared.h"

/* region [room_registry] */

int k__init_room_registry(void);

void k__cleanup_room_registry(void);

/* endregion */

/* region [room] */

struct k_room {

    struct k_asset_registry_node registry_node;

    size_t room_id;

    struct k_step_callback_manager  step_begin_callback_manager;
    struct k_alarm_callback_manager alarm_callback_manager;
    struct k_step_callback_manager  step_callback_manager;
    struct k_draw_callback_manager  draw_callback_manager;
    struct k_step_callback_manager  step_end_callback_manager;

    struct k_list callback_list;

    int  (*fn_init)(void *params);
    void (*fn_fini)(void);
    void (*fn_enter)(void);
    void (*fn_leave)(void);

    struct k_object_pool object_pool;

    float room_w;
    float room_h;

    float view_x;
    float view_y;
    float view_w;
    float view_h;

    uint64_t step_interval_ms;

    unsigned int game_loop;

    void *data;
};

/* endregion */

/* region [room_goto] */

void k__init_room_stack(void);

void k__cleanup_room_stack(void);

struct k_room *k__room_stack_get_top(void);

/* endregion */

/* region [room_run] */

void k__room_run(struct k_room *room);

/* endregion */

#endif
