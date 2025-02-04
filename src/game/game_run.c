#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "k/flow_step.h"
#include "k/log.h"

#include "k/game.h"
#include "_internal_.h"

struct k__game_context * const k__game = &(struct k__game_context) {
    .window       = NULL,
    .renderer     = NULL,
    .quit_game    = 0,
    .current_room = NULL,
};

const struct k_game_config K_GAME_CONFIG_INIT = {
    .window_h = 480,
    .window_w = 640,
    .fn_setup_game = NULL
};

static int check_game_config(const struct k_game_config *config) {

    #define GAME_CONFIG_ASSERT(cond) \
        do { \
            if ( ! (cond)) { \
                k_log_error("Invalid game config, assert( " #cond " )"); \
                return -1; \
            } \
        } while(0)

    GAME_CONFIG_ASSERT(NULL != config);
    GAME_CONFIG_ASSERT(0 < config->window_h);
    GAME_CONFIG_ASSERT(0 < config->window_w);
    GAME_CONFIG_ASSERT(NULL != config->fn_setup_game);

    #undef GAME_CONFIG_ASSERT
    return 0;
}

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

static int run_setup_callback(void *game_config, void *unused) {
    (void)unused;

    const struct k_game_config *config = game_config;

    int result = config->fn_setup_game();
    if (0 != result) {
        k_log_error("fn_setup_game() return %d", result);
        return -1;
    }

    return 0;
}

static const struct k_flow_step steps_to_init[] = {
    { NULL, init_SDL,           deinit_SDL           },
    { NULL, init_room_registry, deinit_room_registry },
    { NULL, run_setup_callback, NULL                 },
};

#define array_len(x) (sizeof(x) / sizeof(x[0]))

static int init_game(const struct k_game_config *config) {

    if (0 != check_game_config(config))
        return -1;

    size_t steps_num = array_len(steps_to_init);
    size_t inited_num = k_execute_flow_forward(steps_to_init, steps_num, (void *)config);

    if (inited_num != steps_num) {
        k_log_error("Failed to initialize game");
        goto err;
    }

    return 0;

err:
    k_execute_flow_backward(steps_to_init, inited_num, NULL);
    return -1;
}

static void deinit_game(void) {
    k_execute_flow_backward(steps_to_init, array_len(steps_to_init), NULL);
}

static void run_room(void) {

    struct k__room *room = k_room_stack_get_top();
    if (NULL == room) {
        k_log_error("No room to run");
        return;
    }

    k__run_room(room);
}

int k_run_game(const struct k_game_config *config) {

    if (0 != init_game(config))
        return -1;

    run_room();

    deinit_game();
    return 0;
}
