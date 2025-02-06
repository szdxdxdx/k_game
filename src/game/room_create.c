#include "k/alloc.h"
#include "k/log.h"
#include "k/list.h"

#include "k/game.h"
#include "_internal_.h"

/* region [room registry] */

struct k__room_registry {
    struct k_list rooms_list;
};

static struct k__room_registry *registry = &(struct k__room_registry){};

static void room_registry_init(void) {
    k_init_list(&registry->rooms_list);
}

static void room_registry_deinit(void) {

    struct k_list_node *iter, *next;
    for(k_list_for_each_s(&registry->rooms_list, iter, next)) {
        struct k_room *room = container_of(iter, struct k_room, room_node);

        k_destroy_room(room);
    }
}

static struct k_room *room_registry_find_by_name(const char *room_name) {

    struct k_list_node *iter;
    for(k_list_for_each(&registry->rooms_list, iter)) {
        struct k_room *room = container_of(iter, struct k_room, room_node);

        if (0 == strcmp(room->name, room_name))
            return room;
    }

    return NULL;
}

static int room_registry_add(struct k_room *room) {

    if (NULL != room_registry_find_by_name(room->name)) {
        k_log_error("The room named \"%s\" already exists", room->name);
        return -1;
    }

    struct k__room_node *room_node = &room->room_node;

    k_list_add_tail(&registry->rooms_list, &room_node->iter_node);
    return 0;
}

static void room_registry_del(struct k_room *room) {
    struct k__room_node *room_node = &room->room_node;
    k_list_del(&room_node->iter_node);
}

/* endregion */

/* region [init] */

static int is_initialized = 0;

int k__init_room_registry(const struct k_game_config *config) {

    if (is_initialized)
        return -1;

    room_registry_init();

    is_initialized = 1;
    return 0;
}

void k__deinit_room_registry(void) {

    if ( ! is_initialized)
        return;

    room_registry_deinit();

    is_initialized = 0;
}

/* endregion */

/* region [create room] */

const struct k_room_config K_ROOM_CONFIG_INIT = {
    .room_name        = NULL,
    .data_size        = 0,
    .steps_per_second = 120,
    .fn_create  = NULL,
    .fn_destroy = NULL,
    .fn_enter   = NULL,
    .fn_leave   = NULL,
    .fn_update    = NULL,
    .fn_draw    = NULL,
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
    K__ROOM_CONFIG_ASSERT(NULL != config->fn_create || NULL != config->fn_enter);

    #undef K__ROOM_CONFIG_ASSERT

    if (NULL == config->room_name || '\0' == config->room_name[0]) {
        k_log_error("Invalid room config. The room name cannot be empty");
        return -1;
    }

    return 0;
}

struct k_room *k_create_room(const struct k_room_config *config) {
    k_log_trace("Creating room { .name=\"%s\" }...", config->room_name);

    if (0 != (check_config(config)))
        goto err_invalid_config;

    struct k_room *room = k_malloc(sizeof(struct k_room));
    if (NULL == room)
        goto err_malloc_room;

    room->name = config->room_name;
    if (0 != room_registry_add(room))
        goto err_registry_add;

    if (0 == config->data_size)
        room->data = NULL;
    else {
        if (NULL == (room->data = k_malloc(config->data_size)))
            goto err_malloc_room_data;
    }

    room->fn_create      = config->fn_create;
    room->fn_destroy     = config->fn_destroy;
    room->fn_enter       = config->fn_enter;
    room->fn_leave       = config->fn_leave;
    room->fn_update      = config->fn_update;
    room->fn_draw        = config->fn_draw;
    room->frame_interval = (Uint32)(1000 / config->steps_per_second);
    room->game_loop      = 0;
    room->delta_ms       = 0;
    room->current_time   = 0;

    if (NULL != room->fn_create) {
        int result = room->fn_create(room);
        if (0 != result) {
            k_log_error("Room fn_create() callback returned %d", result);
            goto err_fn_create;
        }
    }

    k_log_trace("Room { .name=\"%s\" } created", room->name);
    return room;

err_fn_create:
    k_free(room->data);

err_malloc_room_data:
    room_registry_del(room);

err_registry_add:
    k_free(room);

err_malloc_room:
err_invalid_config:

    k_log_error("Failed to create room { .name=\"%s\" }", config->room_name);
    return NULL;
}

/* endregion */

/* region [destroy room] */

void k_destroy_room(struct k_room *room) {
    k_log_trace("Destroying room { .name=\"%s\" }", room->name);

    if (NULL != room->fn_destroy)
        room->fn_destroy(room);

    /* ... */

    room_registry_del(room);
    k_free(room->data);
    k_free(room);

    k_log_trace("Room destroyed");
}

/* endregion */
