#ifndef K_GAME_H
#define K_GAME_H

/* core */
#include "./k_game/core/k_rect.h"
#include "./k_game/core/k_alloc.h"
#include "./k_game/core/k_time.h"
#include "./k_game/core/k_keyboard.h"
#include "./k_game/core/k_mouse.h"
#include "./k_game/core/k_image.h"
#include "./k_game/core/k_sound.h"
#include "./k_game/core/k_sprite.h"
#include "./k_game/core/k_room.h"
#include "./k_game/core/k_object.h"
#include "./k_game/core/k_component.h"

/* ext */
#include "./k_game/ext/k_wasd.h"
#include "./k_game/ext/k_sprite_renderer.h"
#include "./k_game/ext/k_collision.h"
#include "./k_game/ext/k_position.h"
#include "./k_game/ext/k_state_machine.h"
#include "./k_game/ext/k_behavior_tree.h"

/* tmp */
#include "./k_game/tmp/k_game_tmp.h"

struct k_game_config {

    const char *window_title;

    int window_w;
    int window_h;

    int canvas_w;
    int canvas_h;

    int (*fn_init)(void);

    void (*fn_fini)(void);
};

#define K_GAME_CONFIG_INIT \
{ \
    .window_title = "",   \
    .window_h     = 480,  \
    .window_w     = 640,  \
    .canvas_w     = 0,    \
    .canvas_h     = 0,    \
    .fn_init      = NULL, \
    .fn_fini      = NULL, \
}

int k_run_game(const struct k_game_config *config);

#endif
