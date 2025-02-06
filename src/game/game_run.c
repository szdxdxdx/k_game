#include "k/flow_step.h"
#include "k/log.h"

#include "k/game.h"
#include "_internal_.h"

/* region [game context] */

struct k__game_context * const k__game = &(struct k__game_context) {
    .window       = NULL,
    .renderer     = NULL,
    .quit_game    = 0,
    .current_room = NULL,
};

/* endregion */

/* region [game config] */

const struct k_game_config K_GAME_CONFIG_INIT = {
    .window_h    = 480,
    .window_w    = 640,
    .fn_init     = NULL,
    .fn_shutdown = NULL,
};

static int check_game_config(const struct k_game_config *config) {

    #define K__GAME_CONFIG_ASSERT(cond) \
        do { \
            if ( ! (cond)) { \
                k_log_error("Invalid game config, assert( " #cond " )"); \
                return -1; \
            } \
        } while(0)

    K__GAME_CONFIG_ASSERT(NULL != config);
    K__GAME_CONFIG_ASSERT(0 < config->window_h);
    K__GAME_CONFIG_ASSERT(0 < config->window_w);
    K__GAME_CONFIG_ASSERT(NULL != config->fn_init);

    #undef K__GAME_CONFIG_ASSERT
    return 0;
}

/* endregion */

/* region [init && deinit] */

static int init_or_deinit_game(const struct k_game_config *config, int is_init) {

    if ( ! is_init)
        goto deinit;

    if (0 != k__init_SDL(config))
        goto err_init_SDL;

    if (0 != k__init_room_registry(config))
        goto err_init_room_registry;

    if (0 != k__init_room_stack(config))
        goto err_init_room_stack;

    int result = config->fn_init();
    if (0 != result) {
        k_log_error("Failed to start game. Game fn_init() callback return %d", result);
        goto err_fn_init;
    }

    return 0;

deinit:
    if (NULL != config->fn_shutdown)
        config->fn_shutdown();

err_fn_init:
    k__deinit_room_stack();

err_init_room_stack:
    k__deinit_room_registry();

err_init_room_registry:
    k__deinit_SDL();

err_init_SDL:
    return is_init ? -1 : 0;
}

/* endregion */

int k_run_game(const struct k_game_config *config) {
    k_log_trace("Initializing Game...");

    if (0 != check_game_config(config))
        return -1;

    if (0 != init_or_deinit_game(config, 1)) {
        k_log_error("Failed to initialize game");
        return -1;
    }

    k_log_trace("Game initialized. Game started...");

    struct k_room *room = k_room_stack_get_top();
    if (NULL == room) {
        k_log_error("No room to run");
        goto end;
    }

    k__run_room(room);

end:
    k_log_trace("Game end. Deinitializing Game...");

    init_or_deinit_game(config, 0);

    k_log_trace("Game deinitialized");
    return 0;
}
