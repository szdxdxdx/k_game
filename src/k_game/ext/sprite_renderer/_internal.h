#ifndef K__SPRITE_RENDERER_INTERNAL_H
#define K__SPRITE_RENDERER_INTERNAL_H

#include <stddef.h>

#include "k_game.h"

#include "./_shared.h"

enum renderer_transform {
    transform_none     = 0,
    transform_scale_x  = 1 << 0,
    transform_scale_y  = 1 << 1,
    transform_rotate   = 1 << 2,
    transform_flip_x   = 1 << 3,
    transform_flip_y   = 1 << 4,
};

struct k_sprite_renderer {
    struct k_component *component;

    int z_group;
    int z_layer;
    struct k_component_callback *cb_draw_sprite;

    struct k_component_callback *cb_debug_draw;

    float *x;
    float *y;

    struct k_sprite *sprite;

    int loop_count;
    void (*fn_loop_callback)(struct k_object *object);

    size_t frame_idx;
    float  timer;
    float  speed;

    float   scaled_w;
    float   scaled_h;
    float   angle;
    uint8_t transform_flags;
};

void k__sprite_renderer_reset(struct k_sprite_renderer *renderer);

#endif
