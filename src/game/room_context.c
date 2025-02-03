#include <limits.h>

#include "k/log.h"
#include "k/list.h"
#include "k/alloc.h"

#include "k/game.h"
#include "_internal_.h"

const struct k_room_config K_ROOM_CONFIG_INIT = {
    .name             = "",
    .steps_per_second = 60,
    .fn_create_event  = NULL,
    .fn_destroy_event = NULL,
    .fn_entry_event   = NULL,
    .fn_leave_event   = NULL,
    .fn_step_event    = NULL,
    .fn_draw_event    = NULL,
};

size_t k_create_room(const struct k_room_config *config) {

    struct k_room *room = k_malloc(sizeof(*room));
    if (NULL == room)
        return SIZE_MAX;

    room->fn_destroy_event = config->fn_destroy_event;
    room->fn_entry_event   = config->fn_entry_event;
    room->fn_leave_event   = config->fn_leave_event;
    room->fn_step_event    = config->fn_step_event;
    room->fn_draw_event    = config->fn_draw_event;
    room->frame_interval   = (Uint32)(1000 / config->steps_per_second);
    room->game_loop        = 0;
    room->ctx.current_time = 0;
    room->ctx.delta_ms     = 0;

    room->registry_node.name = config->name;
    if (0 != k_room_registry_add(&room->registry_node)) {
        k_free(room);
        return SIZE_MAX;
    }

    if (NULL != config->fn_create_event)
        config->fn_create_event(&room->ctx);

    return room->registry_node.idx;
}
