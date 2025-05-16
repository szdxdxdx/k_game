#ifndef K__SPRITE_RENDERER_INTERNAL_H
#define K__SPRITE_RENDERER_INTERNAL_H

#include <stddef.h>

#include "k_game/core/k_sprite.h"
#include "k_game/core/k_component.h"
#include "k_game/core/k_callback.h"

#include "k_game/ext/k_sprite_renderer.h"
#include "./k_sprite_renderer_type_register.h"

enum renderer_transform {
    transform_none     = 0,
    transform_scaled_w = 1 << 0,
    transform_scaled_h = 1 << 1,
    transform_scale_x  = 1 << 2,
    transform_scale_y  = 1 << 3,
    transform_rotate   = 1 << 4,
    transform_flip_x   = 1 << 5,
    transform_flip_y   = 1 << 6,
};

struct k_sprite_renderer {
    struct k_component *component;

    int z_group;
    int z_layer;
    struct k_callback *cb_draw_sprite;

    struct k_callback *cb_debug_draw;

    float *x;
    float *y;

    struct k_sprite *sprite;

    int loop_count;
    void (*fn_loop_callback)(struct k_object *object);

    size_t frame_idx;
    float  timer;
    float  speed;

    float scaled_w;
    float scaled_h;
    float scale_x;
    float scale_y;
    float angle;
    int   transform_flags;
};

#endif
