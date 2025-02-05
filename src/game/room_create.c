#include <limits.h>

#include "k/alloc.h"
#include "k/log.h"
#include "k/list.h"

#include "k/game.h"
#include "_internal_.h"

/* region [room registry] */

static void destroy_room(struct k__room *room);

static struct k__room_registry *registry = &(struct k__room_registry){};

static void registry_init(void) {
    registry->room_id_counter = 0;
    k_init_list(&registry->rooms_iter_list);
}

static void registry_deinit(void) {

    struct k__room_registry_node *room_node;

    struct k_list_node *iter, *next;
    for(k_list_for_each_s(&registry->rooms_iter_list, iter, next)) {
        room_node = container_of(iter, struct k__room_registry_node, iter_node);

        struct k__room *room = room_node->room;
        k_list_del(&room_node->iter_node);
        k_free(room_node);

        destroy_room(room);
    }

    registry->room_id_counter = 0;
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

            return room;
        }
    }

err:
    k_log_error("Room { .id=%zu } not found", room_id);
    return NULL;
}

static struct k__room *registry_get(size_t room_id) {

    struct k__room_registry_node *room_node;
    struct k_list_node *iter;
    for(k_list_for_each(&registry->rooms_iter_list, iter)) {
        room_node = container_of(iter, struct k__room_registry_node, iter_node);

        if (room_node->room->id == room_id)
            return room_node->room;
    }

    return NULL;
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

    registry_init();

    is_initialized = 1;

    k_log_trace("Room registry initialized");
    return 0;
}

void k__deinit_room_registry(void) {

    if ( ! is_initialized)
        return;

    registry_deinit();
    is_initialized = 0;

    k_log_trace("Room registry deinitialized");
}

/* endregion */

/* region [create room] */

const struct k_room_config K_ROOM_CONFIG_INIT = {
    .room_name        = NULL,
    .steps_per_second = 60,
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
        k_log_error("Invalid room config, room name can not be empty");
        return -1;
    }

    return 0;
}

static int run_room_create_event(struct k__room *room) {

    if (NULL == room->fn_create_event)
        return 0;

    k_log_info("Invoking room fn_create_event() callback...");

    int result = room->fn_create_event(&room->room);
    if (0 != result) {
        k_log_error("Room fn_create_event() callback returned %d", result);
        return -1;
    }

    k_log_info("Room fn_create_event() callback completed");
    return 0;
}

static struct k__room *create_room(const struct k_room_config *config) {

    struct k__room *room = k_malloc(sizeof(struct k__room));
    if (NULL == room)
        goto err;

    room->name = config->room_name;
    room->id   = registry_add(room);
    if (SIZE_MAX == room->id)
        goto err;

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

    if (0 != run_room_create_event(room))
        goto err;

    return room;

err:
    if (NULL != room) {

        if (SIZE_MAX != room->id)
            registry_del(room->id);

        /* ... */

        k_free(room);
    }

    return NULL;
}

size_t k_create_room(const struct k_room_config *config) {
    k_log_info("Creating room { .name=\"%s\" }...", config->room_name);

    if (0 != (check_config(config)))
        goto err;

    struct k__room *room = create_room(config);
    if (NULL == room)
        goto err;

    k_log_info("Room { .name=\"%s\", .id=%zu } created", room->name, room->id);
    return room->id;

err:
    k_log_error("Failed to create room { .name=\"%s\" }", config->room_name);
    return SIZE_MAX;
}

/* endregion */

/* region [destroy room] */

static void run_room_destroy_event(struct k__room *room) {

    if (NULL == room->fn_destroy_event)
        return;

    k_log_info("Invoking room fn_destroy_event() callback...");

    room->fn_destroy_event(&room->room);

    k_log_info("Room fn_destroy_event() callback completed");
}

static void destroy_room(struct k__room *room) {

    run_room_destroy_event(room);

    k_free(room);
}

void k_destroy_room(size_t room_id) {
    k_log_info("Destroying room { .id=%zu }", room_id);

    struct k__room *room = registry_del(room_id);
    if (NULL == room) {
        k_log_error("Failed to destroy room { .id=%zu }", room_id);
        return;
    }

    destroy_room(room);

    k_log_info("Room { .id=%zu } destroyed", room_id);
}

/* endregion */

/* region [get room] */

struct k__room *k__get_room(size_t room_id) {

    struct k__room *room = registry_get(room_id);

    if (NULL == room)
        k_log_error("Room { .id=%zu } not found", room_id);

    return room;
}

/* endregion */
