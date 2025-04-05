#include <stddef.h>

#include "k_log.h"
#include "k_seq_step.h"

#include "k_game.h"

#include "../k_SDL/_public.h"
#include "../image/_public.h"
#include "../sound/_public.h"
#include "../sprite/_public.h"
#include "../room/_public.h"
#include "../component/_public.h"

#include "../../ext/wasd/_public.h"
#include "../../ext/sprite_renderer/_public.h"
#include "../../ext/collision/_public.h"
#include "../../ext/position/_public.h"
#include "../../ext/state_mechine/_public.h"

#include "k_game_config.h"

/* region [steps] */

static int step_init_SDL(void *context) {
    const struct k_game_config *config = context;
    return k__init_SDL(config);
}

static void step_quit_SDL(void *unused) {
    (void)unused;
    k__quit_SDL();
}

static int step_init_image_registry(void *unused) {
    (void)unused;
    return k__init_image_registry();
}

static void step_cleanup_image_registry(void *unused) {
    (void)unused;
    k__cleanup_image_registry();
}

static int step_init_sound_registry(void *unused) {
    (void)unused;

    if (0 != k__init_sound_BGM_registry())
        return -1;

    if (0 != k__init_sound_SFX_registry()) {
        k__cleanup_sound_BGM_registry();
        return -1;
    }

    return 0;
}

static void step_cleanup_sound_registry(void *unused) {
    (void)unused;
    k__cleanup_sound_BGM_registry();
    k__cleanup_sound_SFX_registry();
}

static int step_init_sprite_registry(void *unused) {
    (void)unused;
    return k__init_sprite_registry();
}

static void step_cleanup_sprite_registry(void *unused) {
    (void)unused;
    k__cleanup_sprite_registry();
}

static int step_init_component_registry(void *unused) {
    (void)unused;
    return k__init_component_type_registry();
}

static void step_cleanup_component_registry(void *unused) {
    (void)unused;
    k__cleanup_component_type_registry();
}

static int step_init_component_manager_map(void *unused) {
    (void)unused;
    return k__init_component_manager_map();
}

static void step_free_component_manager_map(void *unused) {
    (void)unused;
    k__deinit_component_manager_map();
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
    return k__init_room_registry();
}

static void step_cleanup_room_registry(void *unused) {
    (void)unused;
    k__cleanup_room_registry();
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
