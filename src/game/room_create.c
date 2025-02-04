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

    struct k__room_registry_node *room_node;
    struct k_list_node *iter_curr;
    struct k_list_node *iter_next;
    for(k_list_for_each_s(&registry->rooms_iter_list, iter_curr, iter_next)) {
        room_node = container_of(iter_curr, struct k__room_registry_node, iter_node);

        struct k__room *room = room_node->room;
        k_list_del(&room_node->iter_node);
        k_free(room_node);

        k_log_trace("Room { .name=\"%s\", .id=%zu } removed from registry", room->name, room->id);

        if (NULL != room->fn_destroy_event) {
            k_log_info("Invoking room fn_destroy_event() callback...");

            room->fn_destroy_event(&room->ctx);

            k_log_info("Room fn_destroy_event() callback completed");
        }

        size_t room_id = room->id;
        k_free(room);

        k_log_info("Room { .id=%zu } destroyed", room_id);
    }

    k_log_trace("Room registry deinitialized");
}

static size_t registry_add(struct k__room *room) {

    struct k__room_registry_node *node = k_malloc(sizeof(*node));
    if (NULL == node)
        return SIZE_MAX;

    size_t room_idx = registry->room_id_counter;

    node->room = room;
    k_list_add_tail(&registry->rooms_iter_list, &node->iter_node);

    k_log_trace("Room { .name=\"%s\", .id=%zu } added to registry", room->name, room_idx);
    return room_idx;
}

static struct k__room *registry_del(size_t room_id) {

    if (SIZE_MAX == room_id)
        goto err;

    struct k__room_registry_node *room_node;
    struct k_list_node *iter;
    for(k_list_for_each(&registry->rooms_iter_list, iter)) {
        room_node = container_of(iter, struct k__room_registry_node, iter_node);

        if (room_node->room->id == room_id) {
            struct k__room *room = room_node->room;

            k_list_del(&room_node->iter_node);
            k_free(room_node);

            k_log_trace("Room { .name=\"%s\", .id=%zu } removed from registry", room->name, room->id);
            return room;
        }
    }

err:
    k_log_error("Room { .id=%zu } not found", room_id);
    return NULL;
}

const struct k_room_config K_ROOM_CONFIG_INIT = {
    .name             = NULL,
    .steps_per_second = 60,
    .fn_create_event  = NULL,
    .fn_destroy_event = NULL,
    .fn_entry_event   = NULL,
    .fn_leave_event   = NULL,
    .fn_step_event    = NULL,
    .fn_draw_event    = NULL,
};

static int check_config(const struct k_room_config *config) {

    #define GAME_CONFIG_ASSERT(cond) \
        do { \
            if ( ! (cond)) { \
                k_log_error("Invalid room config, assert( " #cond " )"); \
                return -1; \
            } \
        } while(0)

    GAME_CONFIG_ASSERT(NULL != config);
    GAME_CONFIG_ASSERT(0 < config->steps_per_second);
    GAME_CONFIG_ASSERT(NULL != config->fn_create_event || NULL != config->fn_entry_event);

    #undef GAME_CONFIG_ASSERT

    if (NULL == config->name || '\0' == config->name[0]) {
        k_log_error("Invalid room config, room name cannot be empty");
        return -1;
    }

    return 0;
}

size_t k_create_room(const struct k_room_config *config) {
    k_log_info("Creating room { .name=\"%s\" }...", config->name);

    if (0 != (check_config(config)))
        goto err;

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

    if (NULL != config->fn_create_event) {
        k_log_info("Invoking room fn_create_event() callback...");

        config->fn_create_event(&room->ctx);

        k_log_info("Room fn_create_event() callback completed");
    }

    k_log_info("Room { .name=\"%s\", .id=%zu } created", room->name, room_id);
    return room_id;

err:
    k_log_error("Failed to create room { .name=\"%s\" }", config->name);
    return SIZE_MAX;
}

void k_destroy_room(size_t room_id) {
    k_log_info("Destroying room { .id=%zu }", room_id);

    struct k__room *room = registry_del(room_id);
    if (NULL == room) {
        k_log_error("Failed to destroy room { .id=%zu }", room_id);
        return;
    }

    if (NULL != room->fn_destroy_event) {
        k_log_info("Invoking room fn_destroy_event() callback...");

        room->fn_destroy_event(&room->ctx);

        k_log_info("Room fn_destroy_event() callback completed");
    }

    k_free(room);

    k_log_info("Room { .id=%zu } destroyed", room_id);
}

struct k__room *k__get_room(size_t room_id) {

    struct k__room_registry_node *room_node;
    struct k_list_node *iter;
    for (k_list_for_each(&registry->rooms_iter_list, iter)) {
        room_node = container_of(iter, struct k__room_registry_node, iter_node);

        if (room_node->room->id == room_id)
            return room_node->room;
    }

    k_log_error("Room { .id=%zu } not found", room_id);
    return NULL;
}
