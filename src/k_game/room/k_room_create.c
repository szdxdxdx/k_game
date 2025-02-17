#include "k_log.h"
#include "k_list.h"

#include "k_game/alloc.h"
#include "k_game/room.h"
#include "./k_room_context.h"

const struct k_room_config K_ROOM_CONFIG_INIT = {
    .room_name        = NULL,
    .room_w           = 600,
    .room_h           = 480,
    .steps_per_second = 60,
    .data_size        = 0,
    .fn_create        = NULL,
    .fn_destroy       = NULL,
};

static int check_config(const struct k_room_config *config) {

    #define K__ROOM_CONFIG_ASSERT(cond) \
        do { \
            if ( ! (cond)) { \
                k_log_error("Invalid room config, assert( " #cond " )"); \
                return -1; \
            } \
        } while(0)

    K__ROOM_CONFIG_ASSERT(0 < config->steps_per_second);

    #undef K__ROOM_CONFIG_ASSERT

    return 0;
}

struct k_room *k_create_room(const struct k_room_config *config, void *params) {

    if (NULL == config)
        goto null_config;

    k_log_trace("Creating room { .name=\"%s\" }...", config->room_name);

    if (0 != (check_config(config)))
        goto invalid_config;

    struct k_room *room = k_malloc(sizeof(struct k_room));
    if (NULL == room)
        goto malloc_room_failed;

    if (0 != k__room_registry_add(&room->room_node, config->room_name))
        goto registry_add_failed;

    if (0 == config->data_size) {
        room->data = NULL;
    } else {
        room->data = k_malloc(config->data_size);
        if (NULL == room->data)
            goto malloc_room_data_failed;
    }

    room->fn_create      = config->fn_create;
    room->fn_destroy     = config->fn_destroy;
    room->frame_interval = (uint32_t)(1000 / config->steps_per_second);
    room->game_loop      = 0;

    k__room_callback_list_init(&room->enter_callbacks);
    k__room_callback_list_init(&room->leave_callbacks);
    k__room_callback_list_init(&room->step_begin_callbacks);
    k__room_callback_list_init(&room->step_callbacks);
    k__room_callback_list_init(&room->step_end_callbacks);
    k__room_init_alarm_callbacks_storage(room);
    k__room_init_draw_callbacks_storage(room);

    if (NULL != room->fn_create) {
        int result = room->fn_create(room, params);
        if (0 != result) {
            k_log_error("Room fn_create() callback returned %d", result);
            goto fn_create_failed;
        }
    }

    k_log_info("Room { .name=\"%s\" } created", k_room_get_name(room));
    return room;

fn_create_failed:
    k__room_callback_list_clean(&room->enter_callbacks);
    k__room_callback_list_clean(&room->leave_callbacks);
    k__room_callback_list_clean(&room->step_begin_callbacks);
    k__room_callback_list_clean(&room->step_callbacks);
    k__room_callback_list_clean(&room->step_end_callbacks);
    k__room_del_all_alarm_callbacks(room);
    k__room_del_all_draw_callbacks(room);

    k_free(room->data);
malloc_room_data_failed:
    k__room_registry_del(&room->room_node);
registry_add_failed:
    k_free(room);

malloc_room_failed:
invalid_config:
    k_log_error("Failed to create room { .name=\"%s\" }", config->room_name);
    return NULL;

null_config:
    k_log_error("Failed to create room. Room config is NULL");
    return NULL;
}

void k__destroy_room(struct k_room *room) {

    k_log_trace("Destroying room { .name=\"%s\" }", k_room_get_name(room));

    if (NULL != room->fn_destroy)
        room->fn_destroy(room);

    /* ... */

    k__room_callback_list_clean(&room->enter_callbacks);
    k__room_callback_list_clean(&room->leave_callbacks);
    k__room_callback_list_clean(&room->step_begin_callbacks);
    k__room_callback_list_clean(&room->step_callbacks);
    k__room_callback_list_clean(&room->step_end_callbacks);
    k__room_del_all_alarm_callbacks(room);
    k__room_del_all_draw_callbacks(room);

    k_free(room->data);
    k__room_registry_del(&room->room_node);
    k_free(room);

    k_log_trace("Room destroyed");
}
