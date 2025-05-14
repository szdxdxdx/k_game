#include <stddef.h>

#define K_LOG_TAG "k_game:init"
#include "k_log.h"

#include "k_seq_step.h"

#include "k_game/core/k_game_run.h"
#include "./k_game_init.h"
#include "./k_game_context.h"

#include "../k_SDL/k_SDL_init.h"
#include "../image/k_image_registry.h"
#include "../sound/k_sound_registry.h"
#include "../font/k_font_registry.h"
#include "../font/k_font_builtin.h"
#include "../sprite/k_sprite_registry.h"
#include "../component/k_component_type_registry.h"
#include "../component/k_component_manager_map.h"
#include "../room/k_room_registry.h"
#include "../room/k_room_stack.h"

#include "../../ext/wasd/k_wasd_type_register.h"
#include "../../ext/sprite_renderer/k_sprite_renderer_type_register.h"
#include "../../ext/collision/k_collision_type_register.h"
#include "../../ext/position/k_position_type_register.h"
#include "../../ext/state_mechine/k_state_machine_register.h"
#include "../../ext/camera/k_camera_type_registry.h"

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

static int step_init_font_registry(void *unused) {
    (void)unused;
    return k__font_registry_init();
}

static void step_cleanup_font_registry(void *unused) {
    (void)unused;
    k__font_registry_cleanup();
}

static int step_load_builtin_font(void *unused) {
    return k__font_load_builtin();
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

    return k__component_type_register_WASD()
        || k__component_type_register_sprite_renderer()
        || k__component_type_register_collision_box()
        || k__component_type_register_position()
        || k__component_type_register_state_machine()
        || k__component_type_register_camera()
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

static void step_clear_room_stack(void *unused) {
    (void)unused;
    k__room_stack_clear();
}

static int step_game_start(void *context) {
    const struct k_game_config *config = context;

    if (NULL == config->on_start) {
        k_log_error("game on_start() callback is null");
        return -1;
    }

    int result = config->on_start();
    if (0 != result) {
        k_log_error("game on_start() callback returned %d", result);
        return -1;
    }

    k__game.quit_game = 0;
    return 0;
}

static void step_game_end(void *context) {
    const struct k_game_config *config = context;

    if (NULL != config->on_end)
        config->on_end();
}

static const struct k_seq_step steps[] = {
    { step_init_SDL,                   step_quit_SDL                   },
    { step_init_image_registry,        step_cleanup_image_registry     },
    { step_init_sound_registry,        step_cleanup_sound_registry     },
    { step_init_font_registry,         step_cleanup_font_registry      },
    { step_load_builtin_font,          NULL                            },
    { step_init_sprite_registry,       step_cleanup_sprite_registry    },
    { step_init_component_registry,    step_cleanup_component_registry },
    { step_init_component_manager_map, step_free_component_manager_map },
    { step_define_components,          NULL                            },
    { step_init_room_registry,         step_cleanup_room_registry      },
    { step_init_room_stack,            step_clear_room_stack           },
    { step_game_start,                 step_game_end                   },
};

/* endregion */

int k__game_init(const struct k_game_config *config) {

    if (NULL == config) {
        k_log_error("`config` is NULL");
        return -1;
    }

    if (0 != k_seq_step_exec(steps, k_seq_step_array_len(steps), (void *)config)) {
        return -1;
    }

    return 0;
}

void k__game_deinit(const struct k_game_config *config) {
    k_seq_step_exec_backward(steps, k_seq_step_array_len(steps), (void *)config);
}
