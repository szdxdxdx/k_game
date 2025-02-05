#include <limits.h>

#include "k/alloc.h"
#include "k/log.h"
#include "k/list.h"

#include "k/game.h"
#include "_internal_.h"

/* region [room registry] */

struct k__room_registry {
    size_t room_id_counter;
    struct k_list rooms_iter_list;
};

static void destroy_room(struct k__room *room);

static struct k__room_registry *registry = &(struct k__room_registry){};

static void room_registry_init(void) {
    registry->room_id_counter = 0;
    k_init_list(&registry->rooms_iter_list);
}

static void room_registry_deinit(void) {

    struct k__room *room;

    struct k_list_node *iter, *next;
    for(k_list_for_each_s(&registry->rooms_iter_list, iter, next)) {
        room = container_of(iter, struct k__room, room_node);

        destroy_room(room);
    }

    registry->room_id_counter = 0;
}

static struct k__room *room_registry_get(size_t room_id) {

    if (SIZE_MAX == room_id)
        return NULL;

    struct k_list_node *iter;
    for(k_list_for_each(&registry->rooms_iter_list, iter)) {
        struct k__room *room = container_of(iter, struct k__room, room_node);

        if (room->id == room_id)
            return room;
    }

    return NULL;
}

static struct k__room *room_registry_find_by_name(const char *room_name) {

    struct k_list_node *iter;
    for(k_list_for_each(&registry->rooms_iter_list, iter)) {
        struct k__room *room = container_of(iter, struct k__room, room_node);

        if (0 == strcmp(room->name, room_name))
            return room;
    }

    return NULL;
}

static int room_registry_add(struct k__room *room) {

    if (NULL != room_registry_find_by_name(room->name)) {
        k_log_error("The room named \"%s\" already exists", room->name);
        return -1;
    }

    struct k__room_registry_node *room_node = &room->room_node;

    k_list_add_tail(&registry->rooms_iter_list, &room_node->iter_node);

    room->id = ++registry->room_id_counter;
    return 0;
}

static void room_registry_del(struct k__room *room) {
    struct k__room_registry_node *room_node = &room->room_node;
    k_list_del(&room_node->iter_node);
}

/* endregion */

/* region [init] */

static int is_initialized = 0;

int k__init_room_registry(const struct k_game_config *config) {
    (void)config;

    if (is_initialized) {
        k_log_error("Room registry already initialized");
        return -1;
    }

    room_registry_init();
    is_initialized = 1;

    k_log_trace("Room registry initialized");
    return 0;
}

void k__deinit_room_registry(void) {

    if ( ! is_initialized)
        return;

    room_registry_deinit();
    is_initialized = 0;

    k_log_trace("Room registry deinitialized");
}

/* endregion */

/* region [create room] */

const struct k_room_config K_ROOM_CONFIG_INIT = {
    .room_name        = NULL,
    .data_size   = 0,
    .steps_per_second = 120,
    .fn_create_event  = NULL,
    .fn_destroy_event = NULL,
    .fn_entry_event   = NULL,
    .fn_leave_event   = NULL,
    .fn_step_event    = NULL,
    .fn_draw_event    = NULL,
};

static int check_config(const struct k_room_config *config) {

    #define K__ROOM_CONFIG_ASSERT(cond) \
        do { \
            if ( ! (cond)) { \
                k_log_error("Invalid room config, assert( " #cond " )"); \
                return -1; \
            } \
        } while(0)

    K__ROOM_CONFIG_ASSERT(NULL != config);
    K__ROOM_CONFIG_ASSERT(0 < config->steps_per_second);
    K__ROOM_CONFIG_ASSERT(NULL != config->fn_create_event || NULL != config->fn_entry_event);

    #undef K__ROOM_CONFIG_ASSERT

    if (NULL == config->room_name || '\0' == config->room_name[0]) {
        k_log_error("Invalid room config. The room name cannot be empty");
        return -1;
    }

    return 0;
}

size_t k_create_room(const struct k_room_config *config) {
    k_log_info("> Creating room { .name=\"%s\" }...", config->room_name);

    if (0 != (check_config(config)))
        goto err_invalid_config;

    struct k__room *room = k_malloc(sizeof(struct k__room));
    if (NULL == room)
        goto err_malloc_room;

    room->name = config->room_name;
    if (0 != room_registry_add(room))
        goto err_registry_add;

    if (0 == config->data_size)
        room->room.data = NULL;
    else {
        if (NULL == (room->room.data = k_malloc(config->data_size)))
            goto err_malloc_room_data;
    }

    room->fn_create_event   = config->fn_create_event;
    room->fn_destroy_event  = config->fn_destroy_event;
    room->fn_entry_event    = config->fn_entry_event;
    room->fn_leave_event    = config->fn_leave_event;
    room->fn_step_event     = config->fn_step_event;
    room->fn_draw_event     = config->fn_draw_event;
    room->frame_interval    = (Uint32)(1000 / config->steps_per_second);
    room->game_loop         = 0;
    room->room.delta_ms     = 0;
    room->room.current_time = 0;

    if (NULL != room->fn_create_event) {
        k_log_info("Invoking room fn_create_event() callback...");

        int result = room->fn_create_event(&room->room);
        if (0 != result) {
            k_log_error("Room fn_create_event() callback returned %d", result);
            goto err_run_create_event;
        }

        k_log_info("Room fn_create_event() callback completed");
    }

    k_log_info("< Room { .name=\"%s\", .id=%zu } created", room->name, room->id);
    return room->id;

err_run_create_event:
    k_free(room->room.data);

err_malloc_room_data:
    room_registry_del(room);

err_registry_add:
    k_free(room);

err_malloc_room:
err_invalid_config:

    k_log_error("< Failed to create room { .name=\"%s\" }", config->room_name);
    return SIZE_MAX;
}

/* endregion */

/* region [destroy room] */

static void destroy_room(struct k__room *room) {
    k_log_info("> Destroying room { .name=\"%s\", .id=%zu }", room->name, room->id);

    if (NULL != room->fn_destroy_event) {
        k_log_info("Invoking room fn_destroy_event() callback...");

        room->fn_destroy_event(&room->room);

        k_log_info("Room fn_destroy_event() callback completed");
    }

    /* ... */

    room_registry_del(room);
    k_free(room->room.data);
    k_free(room);

    k_log_info("< Room destroyed");
}

void k_destroy_room(size_t room_id) {

    struct k__room *room = room_registry_get(room_id);
    if (NULL == room) {
        k_log_error("Failed to destroy room { .id=%zu }. Room not found", room_id);
        return;
    }

    destroy_room(room);
}

/* endregion */

/* region [get room] */

struct k__room *k__get_room(size_t room_id) {

    struct k__room *room = room_registry_get(room_id);

    if (NULL == room)
        k_log_error("Room { .id=%zu } not found", room_id);

    return room;
}

/* endregion */
