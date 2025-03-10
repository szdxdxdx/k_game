#include <stddef.h>

#include "k_log.h"
#include "k_seq_step.h"

#include "k_game_run.h"

#include "../k_SDL/k_SDL.h"
#include "../image/k_image.h"
#include "../sprite/k_sprite.h"
#include "../room/k_room.h"
#include "../component/k_component.h"
#include "../components/k_components_def.h"
#include "../sound/k_sound.h"

/* region [steps] */

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

static void step_quit_SDL(void *data) {
    (void)data;
    k__SDL_quit();
}

static int step_init_image_registry(void *data) {
    (void)data;
    return k__image_registry_init();
}

static void step_cleanup_image_registry(void *data) {
    (void)data;
    k__image_registry_cleanup();
}

static int step_init_sound_registry(void *data) {
    (void)data;
    return k__sound_registry_init();
}

static void step_cleanup_sound_registry(void *data) {
    (void)data;
    k__sound_registry_cleanup();
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

static const struct k_seq_step steps[] = {
    { step_check_config,            NULL                            },
    { step_init_SDL,                step_quit_SDL                   },
    { step_init_image_registry,     step_cleanup_image_registry     },
    { step_init_sound_registry,     step_cleanup_sound_registry     },
    { step_init_sprite_registry,    step_cleanup_sprite_registry    },
    { step_init_component_registry, step_cleanup_component_registry },
    { step_define_components,       NULL                            },
    { step_init_room_registry,      step_cleanup_room_registry      },
    { step_init_room_stack,         step_cleanup_room_stack         },
    { step_call_fn_init,            step_call_fn_cleanup            },
};

/* endregion */

static int init_game(const struct k_game_config *config) {

    if (0 != k_seq_step_exec(steps, k_seq_step_array_len(steps), (void *)config)) {
        k_log_error("Failed to initialize game");
        return -1;
    }

    k_log_info("Game initialized");
    return 0;
}

static void deinit_game(const struct k_game_config *config) {

    k_seq_step_exec_backward(steps, k_seq_step_array_len(steps), (void *)config);
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
