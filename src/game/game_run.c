#include "k/log.h"

#include "k/game.h"
#include "_internal_.h"

/* region [game context] */

struct k__game_context k__game = {
    .window       = NULL,
    .renderer     = NULL,
    .quit_game    = 0,
    .current_room = NULL,
};

/* endregion */

/* region [game config] */

const struct k_game_config K_GAME_CONFIG_INIT = {
    .window_h   = 480,
    .window_w   = 640,
    .fn_init    = NULL,
    .fn_cleanup = NULL,
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

    k_log_trace("Initializing Game...");

    if (0 != k__init_SDL(config))
        goto SDL_init_failed;

    if (0 != k__init_room_registry())
        goto room_registry_init_failed;

    if (0 != k__init_room_stack())
        goto room_stack_init_failed;

    int result = config->fn_init();
    if (0 != result) {
        k_log_error("fn_init() callback returned %d", result);
        goto fn_init_error;
    }


    k_log_trace("Game initialized");
    return 0;

deinit:
    k_log_trace("Deinitializing Game...");

    if (NULL != config->fn_cleanup)
        config->fn_cleanup();

fn_init_error:
    k__deinit_room_stack();

room_stack_init_failed:
    k__deinit_room_registry();

room_registry_init_failed:
    k__deinit_SDL();

SDL_init_failed:

    if (is_init) {
        k_log_error("Failed to initialize game");
        return -1;
    }
    else {
        k_log_trace("Game deinitialized");
        return 0;
    }
}

/* endregion */

static void run_game(const struct k_game_config *config) {
    k_log_info("Game started...");

    struct k_room *room = k_room_stack_get_top();
    if (NULL == room) {
        k_log_error("No room to run");
        goto end;
    }

    k__run_room(room);

end:
    k_log_info("Game end");
}

int k_run_game(const struct k_game_config *config) {

    if (0 != check_game_config(config))
        return -1;

    if (0 != init_or_deinit_game(config, 1))
        return -1;

    run_game(config);

    init_or_deinit_game(config, 0);
    return 0;
}
