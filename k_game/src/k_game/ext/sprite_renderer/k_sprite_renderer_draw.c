#include <assert.h>
#include <limits.h>

#include "k_game/core/k_time.h"
#include "k_game/core/k_canvas.h"

#include "./k_sprite_renderer_internal.h"

/* region [draw_sprite] */

void k__sprite_renderer_on_draw(struct k_component *component) {
    struct k_sprite_renderer *renderer = k_component_get_data(component);

    if (NULL == renderer->sprite)
        return;

    renderer->timer += k_time_get_step_delta() * renderer->speed;

    void (*fn_loop_callback)(struct k_object *object) = NULL;

    struct k_sprite *sprite = renderer->sprite;
    size_t frame_idx_last = k_sprite_get_frames_num(sprite) - 1;
    while (1) {
        float delay = (float)k_sprite_get_frame_delay(sprite, renderer->frame_idx) / 1000.0f;

        if (renderer->timer < delay) {
            break;
        }

        renderer->timer -= delay;

        if (renderer->frame_idx < frame_idx_last) {
            renderer->frame_idx += 1;
        }
        else {
            if (2 <= renderer->loop_count) {
                fn_loop_callback = renderer->fn_loop_callback;
                renderer->loop_count -= 1;
                renderer->frame_idx = 0;
            }
            else if (1 == renderer->loop_count) {
                fn_loop_callback = renderer->fn_loop_callback;
                renderer->loop_count -= 1;
                renderer->frame_idx = frame_idx_last;
            }
            else {
                renderer->frame_idx = frame_idx_last;
            }
        }
    }

    if (transform_none == renderer->transform_flags) {
        k_canvas_room_draw_sprite(sprite, renderer->frame_idx, *(renderer->x), *(renderer->y), NULL);
    }
    else {
        struct k_canvas_draw_sprite_options opts;
        opts.scaled_w = renderer->scaled_w;
        opts.scaled_h = renderer->scaled_h;
        opts.angle    = renderer->angle;
        opts.flip_x   = renderer->transform_flags & transform_flip_x;
        opts.flip_y   = renderer->transform_flags & transform_flip_y;
        k_canvas_room_draw_sprite(sprite, renderer->frame_idx, *(renderer->x), *(renderer->y), &opts);
    }

    if (NULL != fn_loop_callback) {
        fn_loop_callback(k_component_get_object(component));
    }
}

/* endregion */

/* region [ref_sprite] */

void k_sprite_renderer_set_sprite(struct k_sprite_renderer *renderer, struct k_sprite *sprite) {

    if (NULL == sprite) {
        if (NULL == renderer->sprite)
            return;

        renderer->sprite = NULL;

        if ( ! (renderer->transform_flags & transform_scaled_w)) {
            renderer->scaled_w = 0.0f;
        }
        if ( ! (renderer->transform_flags & transform_scaled_h)) {
            renderer->scaled_h = 0.0f;
        }
    }
    else {
        renderer->sprite = sprite;

        renderer->loop_count = INT_MAX;
        renderer->fn_loop_callback = NULL;

        renderer->frame_idx = 0;
        renderer->timer = 0.0f;

        if ( ! (renderer->transform_flags & transform_scaled_w)) {
            renderer->scaled_w = (float)k_sprite_get_w(renderer->sprite) * renderer->scale_x;
        }
        if ( ! (renderer->transform_flags & transform_scaled_h)) {
            renderer->scaled_h = (float)k_sprite_get_h(renderer->sprite) * renderer->scale_y;
        }
    }
}

struct k_sprite *k_sprite_renderer_get_sprite(struct k_sprite_renderer *renderer) {
    return renderer->sprite;
}

/* endregion */

/* region [z_index] */

int k_sprite_renderer_set_z_index(struct k_sprite_renderer *renderer, int z_group, int z_layer) {

    if (z_group == renderer->z_group && z_layer == renderer->z_layer)
        return 0;

    if (NULL != renderer->sprite) {
        struct k_callback *callback = k_component_add_draw_callback(renderer->component, k__sprite_renderer_on_draw, z_group, z_layer);
        if (NULL == callback)
            return -1;

        k_callback_del(renderer->cb_draw_sprite);
        renderer->cb_draw_sprite = callback;
    }

    renderer->z_group = z_group;
    renderer->z_layer = z_layer;
    return 0;
}

int k_sprite_renderer_set_z_group(struct k_sprite_renderer *renderer, int z_group) {
    return k_sprite_renderer_set_z_index(renderer, z_group, renderer->z_layer);
}

int k_sprite_renderer_set_z_layer(struct k_sprite_renderer *renderer, int z_layer) {
    return k_sprite_renderer_set_z_index(renderer, renderer->z_group, z_layer);
}

int k_sprite_renderer_get_z_group(struct k_sprite_renderer *renderer) {
    return renderer->z_group;
}

int k_sprite_renderer_get_z_layer(struct k_sprite_renderer *renderer) {
    return renderer->z_layer;
}

/* endregion */

/* region [speed] */

void k_sprite_renderer_set_speed(struct k_sprite_renderer *renderer, float speed) {

    if (speed <= 0.0f)
        renderer->speed = 0.0f;
    else
        renderer->speed = speed;
}

void k_sprite_renderer_set_duration(struct k_sprite_renderer *renderer, float duration) {

    if (NULL == renderer->sprite || duration <= 0.0f)
        return;

    int sum_delay = 0;

    size_t i = 0;
    size_t frames_num = k_sprite_get_frames_num(renderer->sprite);
    for (; i < frames_num; i++) {
        sum_delay += k_sprite_get_frame_delay(renderer->sprite, i);
    }

    renderer->speed = ((float)(sum_delay) / 1000.0f) / duration;
}

float k_sprite_renderer_get_speed(struct k_sprite_renderer *renderer) {
    return renderer->speed;
}

/* endregion */

/* region [loop_count] */

void k_sprite_renderer_set_loop_count(struct k_sprite_renderer *renderer, int loop_count) {

    if (loop_count <= 1)
        renderer->loop_count = 1;
    else
        renderer->loop_count = loop_count;
}

int k_sprite_renderer_get_loop_count(struct k_sprite_renderer *renderer) {
    return renderer->loop_count;
}

void k_sprite_renderer_set_loop_callback(struct k_sprite_renderer *renderer, void (*fn_callback)(struct k_object *object)) {
    renderer->fn_loop_callback = fn_callback;
}

/* endregion */
