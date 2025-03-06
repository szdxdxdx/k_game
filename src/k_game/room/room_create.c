#include "k_log.h"
#include "k_list.h"
#include "k_seq_step.h"

#include "k_game_alloc.h"

#include "../game/game.h"
#include "../object/object.h"

#include "./room.h"

/* region [room_creation_steps] */

struct k_room_creation_context {
    const struct k_room_config *config;
    void *params;
    struct k_room *room;
};

static int step_check_config(void *data) {
    struct k_room_creation_context *ctx = data;
    const struct k_room_config *config = ctx->config;

    const char *err_msg;

#define check_config_assert(cond) \
    do { if ( ! (cond)) { err_msg = "assert( " #cond " )"; goto err; }} while(0)

    check_config_assert(NULL != config);
    check_config_assert(0 < config->room_speed);

    return 0;

err:
    k_log_error("Invalid room config: %s", err_msg);
    return -1;
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
    struct k_room *room = ctx->room;

    k_free(room);
}

static int step_registry_add(void *data) {
    struct k_room_creation_context *ctx = data;
    const struct k_room_config *config = ctx->config;
    struct k_room *room = ctx->room;

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
    const struct k_room_config *config = ctx->config;
    struct k_room *room = ctx->room;

    room->fn_init    = config->fn_init;
    room->fn_cleanup = config->fn_cleanup;
    room->game_loop  = 0;
    room->step_interval_ms = (uint64_t)(1000 / config->room_speed);

    return 0;
}

static int step_create_object_pool(void *data) {
    struct k_room_creation_context *ctx = data;
    struct k_room *room = ctx->room;

    struct k_object_pool *pool = k__object_pool_create();
    if (NULL == pool)
        return -1;

    room->object_pool = pool;
    return 0;
}

static void step_destroy_object_pool(void *data) {
    struct k_room_creation_context *ctx = data;
    struct k_room *room = ctx->room;

    k__object_pool_destroy(room->object_pool);
}

static int step_init_callback_registry(void *data) {
    struct k_room_creation_context *ctx = data;
    struct k_room *room = ctx->room;

    k__room_callback_list_init(&room->enter_callbacks);
    k__room_callback_list_init(&room->leave_callbacks);

    k__room_init_step_begin_callback_registry(room);
    k__room_init_step_callback_registry(room);
    k__room_init_step_end_callback_registry(room);

    k__room_init_alarm_callback_storage(room);

    k__room_init_draw_callback_storage(room);

    return 0;
}

static void step_cleanup_callback_registry(void *data) {
    struct k_room_creation_context *ctx = data;
    struct k_room *room = ctx->room;

    k__room_callback_list_clean(&room->enter_callbacks);
    k__room_callback_list_clean(&room->leave_callbacks);

    k__room_cleanup_step_begin_callback_registry(room);
    k__room_cleanup_step_callback_registry(room);
    k__room_cleanup_step_end_callback_registry(room);

    k__room_cleanup_alarm_callback_storage(room);

    k__room_cleanup_draw_callback_storage(room);
}

static int step_call_fn_create(void *data) {
    struct k_room_creation_context *ctx = data;
    void *params = ctx->params;
    struct k_room *room = ctx->room;

    if (NULL != room->fn_init) {

        struct k_room *tmp = k__game.current_room;
        k__game.current_room = room;
        int result = room->fn_init(room, params);
        k__game.current_room = tmp; /* [?] fn_init() 可能销毁了 tmp 指向的房间 */

        if (0 != result) {
            k_log_error("Room fn_init() callback returned %d", result);
            return -1;
        }
    }

    return 0;
}

static void step_call_fn_destroy(void *data) {
    struct k_room_creation_context *ctx = data;
    struct k_room *room = ctx->room;

    if (NULL != room->fn_cleanup) {
        struct k_room *tmp = k__game.current_room;
        k__game.current_room = room;
        room->fn_cleanup(room);
        k__game.current_room = tmp; /* [?] fn_cleanup() 可能销毁了 tmp 指向的房间 */
    }
}

static const struct k_seq_step room_creation_steps[] = {
    { step_check_config,           NULL                           },
    { step_malloc_room,            step_free_room                 },
    { step_registry_add,           step_registry_del              },
    { step_malloc_room_data,       step_free_room_data            },
    { step_configure_room,         NULL                           },
    { step_init_callback_registry, step_cleanup_callback_registry },
    { step_create_object_pool,     step_destroy_object_pool       },
    { step_call_fn_create,         step_call_fn_destroy           },
};

/* endregion */

struct k_room *k_room_create(const struct k_room_config *config, void *params) {

    struct k_room_creation_context ctx;
    ctx.config = config;
    ctx.params = params;
    ctx.room   = NULL;

    if (0 != k_seq_step_exec_with_rollback(room_creation_steps, k_array_len(room_creation_steps), &ctx)) {
        k_log_error("Failed to create room");
        return NULL;
    }

    return ctx.room;
}

void k__room_destroy(struct k_room *room) {

    struct k_room_creation_context ctx;
    ctx.config = NULL;
    ctx.params = NULL;
    ctx.room   = room;

    k_seq_step_exec_backward(room_creation_steps, k_array_len(room_creation_steps), &ctx);
}

void k_room_destroy(struct k_room *room) {

    if (NULL != room)
        k__room_destroy(room);
}
