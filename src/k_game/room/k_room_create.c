#include "k_log.h"
#include "k_list.h"
#include "k_seq_step.h"

#include "k_game_alloc.h"

#include "../game/k_game_context.h"
#include "../object/k_object.h"

#include "./k_room.h"

/* region [steps] */

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

static int step_malloc_data(void *data) {
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

static void step_free_data(void *data) {
    struct k_room_creation_context *ctx = data;
    struct k_room *room = ctx->room;

    k_free(room->data);
}

static int step_set_properties(void *data) {
    struct k_room_creation_context *ctx = data;
    const struct k_room_config *config = ctx->config;
    struct k_room *room = ctx->room;

    room->fn_init    = config->fn_init;
    room->fn_cleanup = config->fn_cleanup;
    room->game_loop  = 0;
    room->step_interval_ms = (uint64_t)(1000 / config->room_speed);

    return 0;
}

static int step_init_callback_managers(void *data) {
    struct k_room_creation_context *ctx = data;
    struct k_room *room = ctx->room;

    if (0 != k__callback_init_draw_manager(&room->draw_callback_manager))
        return -1;

    k__callback_init_alarm_manager(&room->alarm_callback_manager);
    k__callback_init_step_manager(&room->step_callback_manager);
    return 0;
}

static void step_deinit_callback_managers(void *data) {
    struct k_room_creation_context *ctx = data;
    struct k_room *room = ctx->room;

    k__callback_deinit_alarm_manager(&room->alarm_callback_manager);
    k__callback_deinit_step_manager(&room->step_callback_manager);
    k__callback_deinit_draw_manager(&room->draw_callback_manager);
}

static int step_init_callback_list(void *data) {
    struct k_room_creation_context *ctx = data;
    struct k_room *room = ctx->room;

    k_list_init(&room->callback_list);
    return 0;
}

static void step_del_callbacks(void *data) {
    struct k_room_creation_context *ctx = data;
    struct k_room *room = ctx->room;

    k_room_del_all_callbacks(room);
}

static int step_init_object_pool(void *data) {
    struct k_room_creation_context *ctx = data;
    struct k_room *room = ctx->room;

    return k__object_pool_init(&room->object_pool);
}

static void step_cleanup_object_pool(void *data) {
    struct k_room_creation_context *ctx = data;
    struct k_room *room = ctx->room;

    k__object_pool_cleanup(&room->object_pool);
}

static int step_call_fn_init(void *data) {
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

static void step_call_fn_cleanup(void *data) {
    struct k_room_creation_context *ctx = data;
    struct k_room *room = ctx->room;

    if (NULL != room->fn_cleanup) {
        struct k_room *tmp = k__game.current_room;
        k__game.current_room = room;
        room->fn_cleanup(room);
        k__game.current_room = tmp; /* [?] fn_cleanup() 可能销毁了 tmp 指向的房间 */
    }
}

static const struct k_seq_step steps[] = {
    { step_check_config,           NULL                          },
    { step_malloc_room,            step_free_room                },
    { step_registry_add,           step_registry_del             },
    { step_malloc_data,            step_free_data                },
    { step_set_properties,         NULL                          },
    { step_init_callback_managers, step_deinit_callback_managers },
    { step_init_callback_list,     step_del_callbacks            },
    { step_init_object_pool,       step_cleanup_object_pool      },
    { step_call_fn_init,           step_call_fn_cleanup          },
};

/* endregion */

struct k_room *k_room_create(const struct k_room_config *config, void *params) {

    struct k_room_creation_context ctx;
    ctx.config = config;
    ctx.params = params;
    ctx.room   = NULL;

    if (0 != k_seq_step_exec(steps, k_seq_step_array_len(steps), &ctx)) {
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

    k_seq_step_exec_backward(steps, k_seq_step_array_len(steps), &ctx);
}

void k_room_destroy(struct k_room *room) {

    if (NULL != room)
        k__room_destroy(room);
}
