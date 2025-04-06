#include <stddef.h>

#include "k_log.h"
#include "k_seq_step.h"

#include "k_game.h"

#include "./k_game_init.h"

#include "../k_SDL/_shared.h"
#include "../image/k_image_registry.h"
#include "../sound/k_sound_registry.h"
#include "../sprite/k_sprite_registry.h"
#include "../room/k_room_registry.h"
#include "../room/k_room_stack.h"
#include "../component/k_component_type_registry.h"
#include "../component/k_component_manager_map.h"

#include "../../ext/wasd/_shared.h"
#include "../../ext/sprite_renderer/_shared.h"
#include "../../ext/collision/_shared.h"
#include "../../ext/position/_shared.h"
#include "../../ext/state_mechine/_shared.h"

/* region [steps] */

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

    if (0 != k__sound_bgm_registry_init())
        return -1;

    if (0 != k__sound_sfx_registry_init()) {
        k__sound_bgm_registry_cleanup();
        return -1;
    }

    return 0;
}

static void step_cleanup_sound_registry(void *unused) {
    (void)unused;
    k__sound_bgm_registry_cleanup();
    k__sound_sfx_registry_cleanup();
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
    k__component_manager_map_deinit();
}

static int step_define_components(void *unused) {
    (void)unused;

    return k__define_component_WASD()
           || k__define_component_sprite_renderer()
           || k__define_component_collision_box()
           || k__define_component_position()
           || k__define_component_state_machine()
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

    k__init_room_stack();
    return 0;
}

static void step_cleanup_room_stack(void *unused) {
    (void)unused;
    k__cleanup_room_stack();
}

static int step_call_fn_init(void *context) {
    const struct k_game_config *config = context;

    if (NULL == config->fn_init) {
        k_log_error("Game fn_init() callback is NULL");
        return -1;
    }

    int result = config->fn_init();
    if (0 != result) {
        k_log_error("Game fn_init() callback returned %d", result);
        return -1;
    }

    return 0;
}

static void step_call_fn_fini(void *context) {
    const struct k_game_config *config = context;

    if (NULL != config->fn_fini)
        config->fn_fini();
}

static const struct k_seq_step steps[] = {
    { step_init_SDL,                   step_quit_SDL                   },
    { step_init_image_registry,        step_cleanup_image_registry     },
    { step_init_sound_registry,        step_cleanup_sound_registry     },
    { step_init_sprite_registry,       step_cleanup_sprite_registry    },
    { step_init_component_registry,    step_cleanup_component_registry },
    { step_init_component_manager_map, step_free_component_manager_map },
    { step_define_components,          NULL                            },
    { step_init_room_registry,         step_cleanup_room_registry      },
    { step_init_room_stack,            step_cleanup_room_stack         },
    { step_call_fn_init,               step_call_fn_fini               },
};

/* endregion */

int k__init_game(const struct k_game_config *config) {

    if (NULL == config) {
        k_log_error("Game config is NULL");
        goto err;
    }

    if (0 != k_seq_step_exec(steps, k_seq_step_array_len(steps), (void *)config)) {
        goto err;
    }

    return 0;

err:
    k_log_error("Failed to initialize game");
    return -1;
}

void k__deinit_game(const struct k_game_config *config) {
    k_seq_step_exec_backward(steps, k_seq_step_array_len(steps), (void *)config);
}
