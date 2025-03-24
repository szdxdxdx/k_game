#include <stddef.h>

#include "k_log.h"
#include "k_seq_step.h"

#include "k_game.h"

#include "./core/k_SDL/_public.h"
#include "./core/image/_public.h"
#include "./core/sound/_public.h"
#include "./core/sprite/_public.h"
#include "./core/room/_public.h"
#include "./core/component/_public.h"

#include "./ext/wasd/_public.h"
#include "./ext/sprite_renderer/_public.h"
#include "./ext/collision/_public.h"
#include "./ext/position/_public.h"

/* region [steps] */

static int step_check_config(void *context) {
    const struct k_game_config *config = context;

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

static int step_init_SDL(void *context) {
    const struct k_game_config *config = context;
    return k__SDL_init(config);
}

static void step_quit_SDL(void *unused) {
    (void)unused;
    k__SDL_quit();
}

static int step_init_image_registry(void *unused) {
    (void)unused;
    return k__image_registry_init();
}

static void step_cleanup_image_registry(void *unused) {
    (void)unused;
    k__image_registry_cleanup();
}

static int step_init_sound_registry(void *unused) {
    (void)unused;

    if (0 != k__sound_BGM_registry_init())
        return -1;

    if (0 != k__sound_SFX_registry_init()) {
        k__sound_BGM_registry_cleanup();
        return -1;
    }

    return 0;
}

static void step_cleanup_sound_registry(void *unused) {
    (void)unused;
    k__sound_BGM_registry_cleanup();
    k__sound_SFX_registry_cleanup();
}

static int step_init_sprite_registry(void *unused) {
    (void)unused;
    return k__sprite_registry_init();
}

static void step_cleanup_sprite_registry(void *unused) {
    (void)unused;
    k__sprite_registry_cleanup();
}

static int step_init_component_registry(void *unused) {
    (void)unused;
    return k__component_type_registry_init();
}

static void step_cleanup_component_registry(void *unused) {
    (void)unused;
    k__component_type_registry_cleanup();
}

static int step_init_component_manager_map(void *unused) {
    (void)unused;
    return k__component_manager_map_init();
}

static void step_free_component_manager_map(void *unused) {
    (void)unused;
    k__component_manager_map_free();
}

static int step_define_components(void *unused) {
    (void)unused;

    return k__component_def_WASD()
        || k__component_def_sprite_renderer()
        || k__component_def_collision_box()
        || k__component_def_position()
     ? -1 : 0;
}

static int step_init_room_registry(void *unused) {
    (void)unused;
    return k__room_registry_init();
}

static void step_cleanup_room_registry(void *unused) {
    (void)unused;
    k__room_registry_cleanup();
}

static int step_init_room_stack(void *unused) {
    (void)unused;

    k__room_stack_init();
    return 0;
}

static void step_cleanup_room_stack(void *unused) {
    (void)unused;
    k__room_stack_cleanup();
}

static int step_call_fn_init(void *context) {
    const struct k_game_config *config = context;

    int result = config->fn_init();
    if (0 != result) {
        k_log_error("Game fn_init() callback returned %d", result);
        return -1;
    }

    return 0;
}

static void step_call_fn_cleanup(void *context) {
    const struct k_game_config *config = context;

    if (NULL != config->fn_cleanup)
        config->fn_cleanup();
}

static const struct k_seq_step steps[] = {
    { step_check_config,               NULL                            },
    { step_init_SDL,                   step_quit_SDL                   },
    { step_init_image_registry,        step_cleanup_image_registry     },
    { step_init_sound_registry,        step_cleanup_sound_registry     },
    { step_init_sprite_registry,       step_cleanup_sprite_registry    },
    { step_init_component_registry,    step_cleanup_component_registry },
    { step_init_component_manager_map, step_free_component_manager_map },
    { step_define_components,          NULL                            },
    { step_init_room_registry,         step_cleanup_room_registry      },
    { step_init_room_stack,            step_cleanup_room_stack         },
    { step_call_fn_init,               step_call_fn_cleanup            },
};

/* endregion */

static int init_game(const struct k_game_config *config) {

    if (0 != k_seq_step_exec(steps, k_seq_step_array_len(steps), (void *)config)) {
        k_log_error("Failed to initialize game");
        return -1;
    }

    return 0;
}

static void deinit_game(const struct k_game_config *config) {
    k_seq_step_exec_backward(steps, k_seq_step_array_len(steps), (void *)config);
}

static void run_game() {

    struct k_room *room = k__room_stack_get_top();
    if (NULL == room) {
        k_log_error("No room to run");
        return;
    }

    k__room_run(room);
}

int k_game_run(const struct k_game_config *config) {

    if (0 != init_game(config))
        return -1;

    run_game();

    deinit_game(config);
    return 0;
}
