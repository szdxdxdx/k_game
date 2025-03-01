#include <stddef.h>

#include "k_log.h"
#include "k_seq_step.h"

#include "k_game/game.h"
#include "../SDL/k_SDL_init.h"
#include "../component/k_component_registry.h"
#include "../room/k_room_goto.h"
#include "../room/k_room_registry.h"
#include "../room/k_room_run.h"

/* region [game_initialization_steps] */

static int step_check_config(void *data) {
    const struct k_game_config *config = data;

    const char *err_msg;

    if (NULL == config) {
        err_msg = "assert( NULL != config )";
        goto err;
    }

    if (config->window_h <= 0) {
        err_msg = "assert( 0 < window_h )";
        goto err;
    }

    if (config->window_w <= 0) {
        err_msg = "assert( 0 < window_w )";
        goto err;
    }

    if (NULL == config->fn_init) {
        err_msg = "assert( NULL != fn_init )";
        goto err;
    }

    return 0;

err:
    k_log_error("Invalid game config: %s", err_msg);
    return -1;
}

static int step_init_SDL(void *data) {
    const struct k_game_config *config = data;
    return k__init_SDL(config);
}

static void step_close_SDL(void *data) {
    (void)data;
    k__close_SDL();
}

static int step_init_room_modules(void *data) {
    (void)data;

    if (0 != k__room_registry_init())
        return -1;

    k__room_stack_init();
    return 0;
}

static void step_deinit_room_modules(void *data) {
    (void)data;

    k__room_stack_cleanup();
    k__room_registry_cleanup();
}

static int step_init_component_modules(void *data) {
    (void)data;
    return k__component_registry_init();
}

static void step_deinit_component_modules(void *data) {
    (void)data;
    k__component_registry_cleanup();
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
    { step_check_config,           NULL                          },
    { step_init_SDL,               step_close_SDL                },
    { step_init_room_modules,      step_deinit_room_modules      },
    { step_init_component_modules, step_deinit_component_modules },
    { step_call_fn_init,           step_call_fn_cleanup          },
};

/* endregion */

static int init_game(const struct k_game_config *config) {

    size_t steps_num = k_array_len(game_initialization_steps);
    size_t completed_count = k_execute_steps_forward(game_initialization_steps, steps_num, (void *)config);
    if (completed_count != steps_num)
        goto err;

    k_log_info("Game initialized");
    return 0;

err:
    k_execute_steps_backward(game_initialization_steps, completed_count, (void *)config);
    k_log_error("Failed to initialize game");
    return -1;
}

static void deinit_game(const struct k_game_config *config) {

    size_t steps_num = k_array_len(game_initialization_steps);
    k_execute_steps_backward(game_initialization_steps, steps_num, (void *)config);

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

int k_run_game(const struct k_game_config *config) {

    if (0 != init_game(config))
        return -1;

    run_game();

    deinit_game(config);
    return 0;
}
