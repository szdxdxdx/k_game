#ifndef K_GAME__SPRITE_RENDERER__INTERNAL_H
#define K_GAME__SPRITE_RENDERER__INTERNAL_H

#include <stddef.h>

#include "k_game.h"

#include "./_public.h"

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

    struct k_component_callback *callback_draw_sprite;
    int z_index;

    int debug;
    struct k_component_callback *callback_draw_rect;

    float *x;
    float *y;

    struct k_sprite *sprite;

    int loop;
    void (*fn_loop_callback)(void *data);
    void *loop_callback_data;

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
