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
    .window_h = 480,
    .window_w = 640,
    .fn_setup_game = NULL
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
    K__GAME_CONFIG_ASSERT(NULL != config->fn_setup_game);

    #undef K__GAME_CONFIG_ASSERT
    return 0;
}

/* endregion */

/* region [init && deinit] */

static int init_SDL(void *game_config, void *unused) {
    (void)unused;

    return 0 == k__init_SDL(game_config) ? 0 : -1;
}

static void deinit_SDL(void *game_config, void *unused) {
    (void)game_config;
    (void)unused;

    k__deinit_SDL();
}

static int init_room_registry(void *game_config, void *unused) {
    (void)game_config;
    (void)unused;

    return 0 == k__init_room_registry(game_config) ? 0 : -1;
}

static void deinit_room_registry(void *game_config, void *unused) {
    (void)game_config;
    (void)unused;

    k__deinit_room_registry();
}

static int init_room_stack(void *game_config, void *unused) {
    (void)game_config;
    (void)unused;

    return 0 == k__init_room_stack(game_config) ? 0 : -1;
}

static void deinit_room_stack(void *game_config, void *unused) {
    (void)game_config;
    (void)unused;

    k__deinit_room_stack();
}

static int run_setup_callback(void *game_config, void *unused) {
    (void)unused;

    k_log_info("Invoking game fn_setup_game() callback...");

    const struct k_game_config *config = game_config;

    int result = config->fn_setup_game();
    if (0 != result) {
        k_log_error("Failed to start game. fn_setup_game() return %d", result);
        return -1;
    }

    k_log_info("game fn_setup_game() callback completed");
    return 0;
}

static const struct k_flow_step steps_to_init[] = {
    { NULL, init_SDL,           deinit_SDL           },
    { NULL, init_room_registry, deinit_room_registry },
    { NULL, init_room_stack,    deinit_room_stack    },
    { NULL, run_setup_callback, NULL                 },
};

#define array_len(x) (sizeof(x) / sizeof(x[0]))

static int init_game(const struct k_game_config *config) {

    size_t steps_num = array_len(steps_to_init);
    size_t inited_num = k_execute_flow_forward(steps_to_init, steps_num, (void *)config);

    if (inited_num != steps_num) {
        k_execute_flow_backward(steps_to_init, inited_num, NULL);
        return -1;
    }

    return 0;
}

static void deinit_game(void) {
    k_execute_flow_backward(steps_to_init, array_len(steps_to_init), NULL);
}

/* endregion */

static void run_room(void) {

    struct k__room *room = k_room_stack_get_top();
    if (NULL == room) {
        k_log_error("No room to run");
        return;
    }

    k__run_room(room);
}

int k_run_game(const struct k_game_config *config) {
    k_log_info("Initializing Game...");

    if (0 != check_game_config(config))
        return -1;

    if (0 != init_game(config)) {
        k_log_error("Failed to initialize game");
        return -1;
    }

    k_log_info("Game initialized. Game started...");

    run_room();

    k_log_info("Deinitializing Game...");

    deinit_game();

    k_log_info("Game deinitialized");
    return 0;
}
