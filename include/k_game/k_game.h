#ifndef K_GAME_H
#define K_GAME_H

/* core */
#include "./core/k_rect.h"
#include "./core/k_alloc.h"
#include "./core/k_time.h"
#include "./core/k_keyboard.h"
#include "./core/k_mouse.h"
#include "./core/k_image.h"
#include "./core/k_sound.h"
#include "./core/k_sprite.h"
#include "./core/k_room.h"
#include "./core/k_object.h"
#include "./core/k_component.h"

/* component */
#include "./ext/k_wasd.h"
#include "./ext/k_sprite_renderer.h"
#include "./ext/k_collision.h"
#include "./ext/k_position.h"

/* tmp */
#include "./tmp/k_game_tmp.h"

struct k_game_config {

    const char *window_title;

    int window_w, window_h;

    int (*fn_init)(void);

    void (*fn_cleanup)(void);
};

#define K_GAME_CONFIG_INIT \
{ \
    .window_title = "k_game", \
    .window_h     = 480,      \
    .window_w     = 640,      \
    .fn_init      = NULL,     \
    .fn_cleanup   = NULL,     \
}

int k_run_game(const struct k_game_config *config);

#endif
