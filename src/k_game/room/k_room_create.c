#include "k_log.h"
#include "k_list.h"
#include "k_seq_step.h"

#include "k_game_alloc.h"

#include "../game/k_game_context.h"
#include "../object/k_object.h"

#include "./k_room.h"
#include "../component/k_component.h"

/* region [steps] */

struct step_context {
    const struct k_room_config *config;
    void *params;
    struct k_room *room;
};

static int step_check_config(void *context) {
    struct step_context *ctx = context;
    const struct k_room_config *config = ctx->config;

    const char *err_msg;

#define check_config_assert(cond) \
    do { if ( ! (cond)) { err_msg = "assert( " #cond " )"; goto err; }} while(0)

    check_config_assert(NULL != config);
    check_config_assert(0 < config->room_speed);
    check_config_assert(config->data_size < SIZE_MAX - sizeof(struct k_room));

    return 0;

err:
    k_log_error("Invalid room config: %s", err_msg);
    return -1;
}

static int step_malloc(void *context) {
    struct step_context *ctx = context;
    const struct k_room_config *config = ctx->config;

#define ptr_offset(p, offset) ((void *)((char *)(p) + (offset)))

    ctx->room = k_malloc(sizeof(struct k_room) + config->data_size);
    if (NULL == ctx->room)
        return -1;

    if (0 != config->data_size) {
        ctx->room->data = ptr_offset(ctx->room, sizeof(struct k_room));
    } else {
        ctx->room->data = NULL;
    }

    return 0;
}

static void step_free(void *context) {
    struct step_context *ctx = context;
    struct k_room *room = ctx->room;

    k_free(room);
}

static int step_set_properties(void *context) {
    struct step_context *ctx = context;
    const struct k_room_config *config = ctx->config;
    struct k_room *room = ctx->room;

    room->fn_init    = config->fn_init;
    room->fn_cleanup = config->fn_cleanup;
    room->fn_enter   = config->fn_enter;
    room->fn_leave   = config->fn_leave;

    room->room_w = config->room_w;
    room->room_h = config->room_h;

    room->game_loop = 0;

    room->step_interval_ms = (uint64_t)(1000 / config->room_speed);

    return 0;
}

static int step_init_callback_managers(void *context) {
    struct step_context *ctx = context;
    struct k_room *room = ctx->room;

    if (0 != k__callback_init_draw_manager(&room->draw_callback_manager))
        return -1;

    k__callback_init_step_manager(&room->step_begin_callback_manager);
    k__callback_init_alarm_manager(&room->alarm_callback_manager);
    k__callback_init_step_manager(&room->step_callback_manager);
    k__callback_init_step_manager(&room->step_end_callback_manager);
    return 0;
}

static void step_deinit_callback_managers(void *context) {
    struct step_context *ctx = context;
    struct k_room *room = ctx->room;

    k__callback_deinit_step_manager(&room->step_begin_callback_manager);
    k__callback_deinit_alarm_manager(&room->alarm_callback_manager);
    k__callback_deinit_step_manager(&room->step_callback_manager);
    k__callback_deinit_draw_manager(&room->draw_callback_manager);
    k__callback_deinit_step_manager(&room->step_end_callback_manager);
}

static int step_init_callback_list(void *context) {
    struct step_context *ctx = context;
    struct k_room *room = ctx->room;

    k_list_init(&room->callback_list);
    return 0;
}

static void step_del_all_callbacks(void *unused) {
    (void)unused;
    /* 交由各个 callback manager 清除 */
}

static int step_init_object_pool(void *context) {
    struct step_context *ctx = context;
    struct k_room *room = ctx->room;

    return k__object_pool_init(&room->object_pool);
}

static void step_cleanup_object_pool(void *context) {
    struct step_context *ctx = context;
    struct k_room *room = ctx->room;

    k__object_pool_cleanup(&room->object_pool);
}

static void step_del_component_managers(void *context) {
    struct step_context *ctx = context;
    struct k_room *room = ctx->room;

    k__room_del_all_component_managers(room);
}

static size_t id_counter = 0;

static int step_registry_add(void *context) {
    struct step_context *ctx = context;
    struct k_room *room = ctx->room;

    k__room_registry_add(room);

    room->room_id = id_counter++;
    return 0;
}

static void step_registry_del(void *context) {
    struct step_context *ctx = context;
    struct k_room *room = ctx->room;

    k__room_registry_del(room);

    if (id_counter == room->room_id)
        id_counter--; /* [?] 若 `fn_init()` 初始化失败则回收 id */
}

static int step_call_fn_init(void *context) {
    struct step_context *ctx = context;
    struct k_room *room = ctx->room;

    if (NULL == room->fn_init)
        return 0;

    void *params = ctx->params;

    struct k_room *tmp = k__game.current_room;
    k__game.current_room = room;

    int result = room->fn_init(params);

    /* [?] fn_init() 可能销毁了 tmp 指向的房间 */
    k__game.current_room = tmp;

    if (0 != result) {
        k_log_error("Room fn_init() callback returned %d", result);
        return -1;
    }

    return 0;
}

static void step_call_fn_cleanup(void *context) {
    struct step_context *ctx = context;
    struct k_room *room = ctx->room;

    if (NULL == room->fn_cleanup)
        return;

    struct k_room *tmp = k__game.current_room;
    k__game.current_room = room;

    room->fn_cleanup();

    /* [?] fn_cleanup() 可能销毁了 tmp 指向的房间 */
    k__game.current_room = tmp;
}

static const struct k_seq_step steps[] = {
    { step_check_config,           NULL                          },
    { step_malloc,                 step_free                     },
    { step_set_properties,         NULL                          },
    { step_init_callback_managers, step_deinit_callback_managers },
    { step_init_callback_list,     step_del_all_callbacks        },
    { step_init_object_pool,       step_cleanup_object_pool      },
    { NULL,                        step_del_component_managers   },
    { step_registry_add,           step_registry_del             },
    { step_call_fn_init,           step_call_fn_cleanup          },
};

/* endregion */

struct k_room *k_room_create(const struct k_room_config *config, void *params) {

    struct step_context ctx;
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

    struct step_context ctx;
    ctx.config = NULL;
    ctx.params = NULL;
    ctx.room   = room;

    k_seq_step_exec_backward(steps, k_seq_step_array_len(steps), &ctx);
}

void k_room_destroy(struct k_room *room) {

    if (NULL != room)
        k__room_destroy(room);
}
