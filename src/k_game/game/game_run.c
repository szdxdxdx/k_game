#include <stddef.h>

#include "k_log.h"
#include "k_seq_step.h"

#include "k_game/k_SDL.h"
#include "k_game/image_asset.h"
#include "k_game/sprite_asset.h"
#include "k_game/room_context.h"
#include "k_game/room_goto.h"
#include "k_game/component_type.h"
#include "k_game_run.h"

/* region [game_initialization_steps] */

static int step_check_config(void *data) {
    const struct k_game_config *config = data;

    const char *err_msg;

#define check_config_assert(cond) \
    do { if ( ! (cond)) { err_msg = "assert( " #cond " )"; goto err; }} while(0)

    check_config_assert(NULL != config);
    check_config_assert(0 < config->window_h);
    check_config_assert(0 < config->window_w);
    check_config_assert(NULL != config->fn_init);

    return 0;

err:
    k_log_error("Invalid game config: %s", err_msg);
    return -1;
}

static int step_init_SDL(void *data) {
    const struct k_game_config *config = data;
    return k__SDL_init(config);
}

static void step_close_SDL(void *data) {
    (void)data;
    k__SDL_close();
}

static int step_init_image_registry(void *data) {
    (void)data;
    return k__image_registry_init();
}

static void step_cleanup_image_registry(void *data) {
    (void)data;
    k__image_registry_cleanup();
}

static int step_init_sprite_registry(void *data) {
    (void)data;
    return k__sprite_registry_init();
}

static void step_cleanup_sprite_registry(void *data) {
    (void)data;
    k__sprite_registry_cleanup();
}

static int step_init_component_registry(void *data) {
    (void)data;

    return k__component_registry_init();
}

static void step_cleanup_component_registry(void *data) {
    (void)data;
    k__component_registry_cleanup();
}

static int step_define_components(void *data) {
    (void)data;

    if (0 != k__component_def_WASD())            return -1;
    if (0 != k__component_def_sprite_renderer()) return -1;

    return 0;
}

static int step_init_room_registry(void *data) {
    (void)data;
    return k__room_registry_init();
}

static void step_cleanup_room_registry(void *data) {
    (void)data;
    k__room_registry_cleanup();
}

static int step_init_room_stack(void *data) {
    (void)data;

    k__room_stack_init();
    return 0;
}

static void step_cleanup_room_stack(void *data) {
    (void)data;
    k__room_stack_cleanup();
}

static int step_call_fn_init(void *data) {
    const struct k_game_config *config = data;

    int result = config->fn_init();
    if (0 != result) {
        k_log_error("Game fn_init() callback returned %d", result);
        return -1;
    }

    return 0;
}

static void step_call_fn_cleanup(void *data) {
    const struct k_game_config *config = data;

    if (NULL != config->fn_cleanup)
        config->fn_cleanup();
}

static const struct k_seq_step game_initialization_steps[] = {
    { step_check_config,            NULL                            },
    { step_init_SDL,                step_close_SDL                  },
    { step_init_image_registry,     step_cleanup_image_registry     },
    { step_init_sprite_registry,    step_cleanup_sprite_registry    },
    { step_init_component_registry, step_cleanup_component_registry },
    { step_define_components,       NULL                            },
    { step_init_room_registry,      step_cleanup_room_registry      },
    { step_init_room_stack,         step_cleanup_room_stack         },
    { step_call_fn_init,            step_call_fn_cleanup            },
};

/* endregion */

static int init_game(const struct k_game_config *config) {

    size_t steps_num = k_array_len(game_initialization_steps);
    size_t completed_count = k_seq_step_exec_forward(game_initialization_steps, steps_num, (void *)config);
    if (completed_count != steps_num)
        goto err;

    k_log_info("Game initialized");
    return 0;

err:
    k_seq_step_exec_backward(game_initialization_steps, completed_count, (void *)config);
    k_log_error("Failed to initialize game");
    return -1;
}

static void deinit_game(const struct k_game_config *config) {

    size_t steps_num = k_array_len(game_initialization_steps);
    k_seq_step_exec_backward(game_initialization_steps, steps_num, (void *)config);

    k_log_info("Game deinitialized");
}

static void run_game() {
    k_log_info("Game started...");

    struct k_room *room = k__room_stack_get_top();
    if (NULL == room) {
        k_log_error("No room to run");
        goto end;
    }

    k__room_run(room);

end:
    k_log_info("Game end");
}

int k_game_run(const struct k_game_config *config) {

    if (0 != init_game(config))
        return -1;

    run_game();

    deinit_game(config);
    return 0;
}
