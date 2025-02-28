#include "k_log.h"
#include "k_list.h"
#include "k_seq_step.h"

#include "k_game/alloc.h"
#include "k_game/room.h"
#include "../game/k_game_context.h"
#include "./k_room_context.h"
#include "./k_room_create.h"

const struct k_room_config K_ROOM_CONFIG_INIT = {
    .room_name  = NULL,
    .room_w     = 600,
    .room_h     = 480,
    .room_speed = 60,
    .data_size  = 0,
    .fn_create  = NULL,
    .fn_destroy = NULL,
};

/* region [steps] */

struct k_room_creation_context {
    const struct k_room_config *config;
    void *params;
    struct k_room *room;
};

static int step_check_config(void *data) {
    struct k_room_creation_context *ctx = data;
    const struct k_room_config *config = ctx->config;

    if (NULL == config) {
        k_log_error("Invalid room config: assert( NULL != config )");
        return -1;
    }

    if (config->room_speed <= 0) {
        k_log_error("Invalid room config: assert( 0 < room_speed )");
        return -1;
    }

    return 0;
}

static int step_malloc_room(void *data) {
    struct k_room_creation_context *ctx = data;

    if (NULL == (ctx->room = k_malloc(sizeof(struct k_room))))
        return -1;
    else
        return 0;
}

static void step_free_room(void *data) {
    struct k_room_creation_context *ctx = data;

    k_free(ctx->room);
}

static int step_registry_add(void *data) {
    struct k_room_creation_context *ctx = data;
    struct k_room *room = ctx->room;
    const struct k_room_config *config = ctx->config;

    return k__room_registry_add(room, config->room_name);
}

static void step_registry_del(void *data) {
    struct k_room_creation_context *ctx = data;
    struct k_room *room = ctx->room;

    k__room_registry_del(room);
}

static int step_malloc_room_data(void *data) {
    struct k_room_creation_context *ctx = data;
    const struct k_room_config *config = ctx->config;
    struct k_room *room = ctx->room;

    if (0 == config->data_size) {
        room->data = NULL;
    } else {
        room->data = k_malloc(config->data_size);
        if (NULL == room->data)
            return -1;
    }

    return 0;
}

static void step_free_room_data(void *data) {
    struct k_room_creation_context *ctx = data;
    struct k_room *room = ctx->room;

    k_free(room->data);
}

static int step_configure_room(void *data) {
    struct k_room_creation_context *ctx = data;
    struct k_room *room = ctx->room;
    const struct k_room_config *config = ctx->config;

    room->fn_create  = config->fn_create;
    room->fn_destroy = config->fn_destroy;
    room->game_loop  = 0;
    room->step_interval_ms = (uint64_t)(1000 / config->room_speed);

    return 0;
}

static int step_init_object_pool(void *data) {
    struct k_room_creation_context *ctx = data;
    struct k_room *room = ctx->room;

    k__room_init_object_pool(room);
    return 0;
}

static void step_deinit_object_pool(void *data) {
    struct k_room_creation_context *ctx = data;
    struct k_room *room = ctx->room;

    k__room_deinit_object_pool(room);
}

static int step_init_callbacks_storage(void *data) {
    struct k_room_creation_context *ctx = data;
    struct k_room *room = ctx->room;

    k__room_callback_list_init(&room->enter_callbacks);
    k__room_callback_list_init(&room->leave_callbacks);
    k__room_callback_list_init(&room->step_begin_callbacks);
    k__room_callback_list_init(&room->step_callbacks);
    k__room_callback_list_init(&room->step_end_callbacks);
    k__room_init_alarm_callbacks_storage(room);
    k__room_init_draw_callbacks_storage(room);

    return 0;
}

static void step_cleanup_callbacks_storage(void *data) {
    struct k_room_creation_context *ctx = data;
    struct k_room *room = ctx->room;

    k__room_callback_list_clean(&room->enter_callbacks);
    k__room_callback_list_clean(&room->leave_callbacks);
    k__room_callback_list_clean(&room->step_begin_callbacks);
    k__room_callback_list_clean(&room->step_callbacks);
    k__room_callback_list_clean(&room->step_end_callbacks);
    k__room_del_all_alarm_callbacks(room);
    k__room_del_all_draw_callbacks(room);
}

static int step_call_fn_create(void *data) {
    struct k_room_creation_context *ctx = data;
    struct k_room *room = ctx->room;
    void *params = ctx->params;

    if (NULL != room->fn_create) {

        struct k_room *tmp = k__game.current_room;
        k__game.current_room = room;
        int result = room->fn_create(room, params);
        k__game.current_room = tmp; /* TODO: fn_create() 可能销毁了 tmp 指向的房间？ */

        if (0 != result) {
            k_log_error("Room fn_create() callback returned %d", result);
            return -1;
        }
    }

    return 0;
}

static void step_call_fn_destroy(void *data) {
    struct k_room_creation_context *ctx = data;
    struct k_room *room = ctx->room;

    if (NULL != room->fn_destroy) {
        struct k_room *tmp = k__game.current_room;
        k__game.current_room = room;
        room->fn_destroy(room);
        k__game.current_room = tmp; /* TODO: fn_destroy() 可能销毁了 tmp 指向的房间？ */
    }
}

static const struct k_seq_step room_creation_steps[] = {
    { step_check_config,           NULL                           },
    { step_malloc_room,            step_free_room                 },
    { step_registry_add,           step_registry_del              },
    { step_malloc_room_data,       step_free_room_data            },
    { step_configure_room,         NULL                           },
    { step_init_callbacks_storage, step_cleanup_callbacks_storage },
    { step_init_object_pool,       step_deinit_object_pool        },
    { step_call_fn_create,         step_call_fn_destroy           },
};

/* endregion */

struct k_room *k_create_room(const struct k_room_config *config, void *params) {

    struct k_room_creation_context ctx;
    ctx.config = config;
    ctx.params = params;
    ctx.room   = NULL;

    size_t steps_num = k_array_len(room_creation_steps);
    size_t completed_count = k_execute_steps_forward(room_creation_steps, steps_num, &ctx);
    if (completed_count != steps_num) {
        k_execute_steps_backward(room_creation_steps, completed_count, &ctx);
        return NULL;
    }

    return ctx.room;
}

void k__destroy_room(struct k_room *room) {

    struct k_room_creation_context ctx;
    ctx.config = NULL;
    ctx.params = NULL;
    ctx.room   = room;

    k_execute_steps_backward(room_creation_steps, k_array_len(room_creation_steps), &ctx);
}
