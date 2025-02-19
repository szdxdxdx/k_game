#include <stddef.h>

#include "k_log.h"

#include "../room/k_room_goto.h"
#include "../room/k_room_registry.h"
#include "../room/k_room_context.h"
#include "k_game/game.h"
#include "./k_game_SDL.h"

/* region [game config] */

const struct k_game_config K_GAME_CONFIG_INIT = {
    .window_title = "k_game",
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

    k_log_info("Initializing Game...");

    if (0 != check_game_config(config))
        goto invalid_config;

    if (0 != k__game_SDL_init(config))
        goto SDL_init_failed;

    k__room_registry_init();
    k__room_stack_init();

    int result = config->fn_init();
    if (0 != result) {
        k_log_error("Game fn_init() callback returned %d", result);
        goto fn_init_failed;
    }

    k_log_info("Game initialized");
    return 0;

deinit:
    k_log_info("Deinitializing Game...");

    if (NULL != config->fn_cleanup)
        config->fn_cleanup();

fn_init_failed:
    k__room_stack_deinit();
    k__room_registry_deinit();
    k__game_SDL_deinit();

SDL_init_failed:
invalid_config:
    if (is_init) {
        k_log_error("Failed to initialize game");
        return -1;
    }
    else {
        k_log_info("Game deinitialized");
        return 0;
    }
}

/* endregion */

static void run_game(void) {
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

    if (NULL == config) {
        k_log_error("Invalid game config. Config is NULL");
        return -1;
    }

    if (0 != init_or_deinit_game(config, 1))
        return -1;

    run_game();

    init_or_deinit_game(config, 0);
    return 0;
}
