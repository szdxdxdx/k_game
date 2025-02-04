#include <limits.h>

#include "k/alloc.h"
#include "k/log.h"
#include "k/list.h"

#include "k/game.h"
#include "_internal_.h"

struct k__room_registry_node {
    struct k_list_node iter_node;
    struct k__room *room;
};

struct k__room_registry {
    size_t room_id_counter;
    struct k_list rooms_iter_list;
};

static struct k__room_registry *registry = &(struct k__room_registry){};

int k__init_room_registry(const struct k_game_config *config) {
    (void)config;

    registry->room_id_counter = 0;
    k_init_list(&registry->rooms_iter_list);

    k_log_trace("Room registry initialized");
    return 0;
}

void k__deinit_room_registry(void) {

    /* TODO */

    k_log_trace("Room registry deinitialized");
}

static size_t registry_add(struct k__room *room) {

    struct k__room_registry_node *node = k_malloc(sizeof(*node));
    if (NULL == node)
        return SIZE_MAX;

    size_t room_idx = registry->room_id_counter;

    node->room = room;
    k_list_add_tail(&registry->rooms_iter_list, &node->iter_node);

    return room_idx;
}

static struct k__room *registry_del(size_t room_id) {

    struct k__room_registry_node *room_node;
    struct k_list_node *list_node;
    for(k_list_for_each(&registry->rooms_iter_list, list_node)) {
        room_node = container_of(list_node, struct k__room_registry_node, iter_node);

        if (room_node->room->id == room_id) {
            k_list_del(&room_node->iter_node);
            k_free(room_node);
            return room_node->room;
        }
    }

    return NULL;
}

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

    struct k__room *room = k_malloc(sizeof(struct k__room));
    if (NULL == room)
        goto err;

    room->name = config->name;
    size_t room_id = registry_add(room);
    if (SIZE_MAX == room_id) {
        k_free(room);
        goto err;
    }

    room->id               = room_id;
    room->fn_destroy_event = config->fn_destroy_event;
    room->fn_entry_event   = config->fn_entry_event;
    room->fn_leave_event   = config->fn_leave_event;
    room->fn_step_event    = config->fn_step_event;
    room->fn_draw_event    = config->fn_draw_event;
    room->frame_interval   = (Uint32)(1000 / config->steps_per_second);
    room->game_loop        = 0;
    room->ctx.current_time = 0;
    room->ctx.delta_ms     = 0;

    if (NULL != config->fn_create_event)
        config->fn_create_event(&room->ctx);

    return room_id;

err:
    return SIZE_MAX;
}

void k_destroy_room(size_t room_id) {

    const char *err_msg = NULL;

    if (SIZE_MAX == room_id) {
        err_msg = "Invalid room id";
        goto err;
    }

    struct k__room *room = registry_del(room_id);
    if (NULL == room) {
        err_msg = "Room not found";
        goto err;
    }

    if (NULL != room->fn_destroy_event)
        room->fn_destroy_event(&room->ctx);

    k_log_info("Room with id %zu destroyed", room_id);

    return;

err:
    k_log_error("Failed to destroy room with id %zu: %s", room_id, err_msg);
}

struct k__room *k__get_room(size_t room_id) {

    struct k__room_registry_node *room_node;
    struct k_list_node *list_iterator;
    for (k_list_for_each(&registry->rooms_iter_list, list_iterator)) {
        room_node = container_of(list_iterator, struct k__room_registry_node, iter_node);

        if (room_node->room->id == room_id)
            return room_node->room;
    }

    k_log_error("Room with id %zu not found", room_id);
    return NULL;
}
