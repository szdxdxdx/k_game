#include "./_internal.h"

#include "../../core/sprite/_public.h"

/* region [renderer_draw] */

static void draw_sprite(struct k_component *component) {
    struct k_sprite_renderer *renderer = k_component_get_data(component);

    renderer->timer += k_get_step_delta() * renderer->speed;

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
            if (2 <= renderer->loop) {
                fn_loop_callback = renderer->fn_loop_callback;
                renderer->loop -= 1;
                renderer->frame_idx = 0;
            }
            else if (1 == renderer->loop) {
                fn_loop_callback = renderer->fn_loop_callback;
                renderer->loop -= 1;
                renderer->frame_idx = frame_idx_last;
            }
            else {
                renderer->frame_idx = frame_idx_last;
            }
        }
    }

    if (transform_none == renderer->transform_flags) {
        k_draw_sprite(sprite, renderer->frame_idx, *(renderer->x), *(renderer->y), NULL);
    }
    else {
        struct k_sprite_draw_options opts;
        opts.scaled_w = renderer->scaled_w;
        opts.scaled_h = renderer->scaled_h;
        opts.angle    = renderer->angle;
        opts.flip_x   = renderer->transform_flags & transform_flip_x;
        opts.flip_y   = renderer->transform_flags & transform_flip_y;
        k_draw_sprite(sprite, renderer->frame_idx, *(renderer->x), *(renderer->y), &opts);
    }

    if (NULL != fn_loop_callback) {
        fn_loop_callback(k_component_get_object(component));
    }
}

/* endregion */

/* region [ref_sprite] */

int k_sprite_renderer_set_sprite(struct k_sprite_renderer *renderer, struct k_sprite *sprite) {

    if (NULL == sprite) {

        if (NULL == renderer->sprite)
            return 0;

        k_component_del_callback(renderer->callback_draw_sprite);
        renderer->callback_draw_sprite = NULL;
    }
    else {
        struct k_component_callback *callback = k_component_add_draw_callback(renderer->component, draw_sprite, renderer->z_group, renderer->z_layer);
        if (NULL == callback)
            return -1;

        k_component_del_callback(renderer->callback_draw_sprite);
        renderer->callback_draw_sprite = callback;
    }

    renderer->sprite = sprite;

    k__sprite_renderer_reset(renderer);
    return 0;
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
        struct k_component_callback *callback = k_component_add_draw_callback(renderer->component, draw_sprite, z_group, z_layer);
        if (NULL == callback)
            return -1;

        k_component_del_callback(renderer->callback_draw_sprite);
        renderer->callback_draw_sprite = callback;
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

    if (speed <= 0)
        renderer->speed = 0;
    else
        renderer->speed = speed;
}

float k_sprite_renderer_get_speed(struct k_sprite_renderer *renderer) {

    if (NULL == renderer->sprite)
        return 0;
    else
        return renderer->speed;
}

/* endregion */

/* region [loop] */

void k_sprite_renderer_set_loop(struct k_sprite_renderer *renderer, int loop) {

    if (loop <= 1)
        renderer->loop = 1;
    else
        renderer->loop = loop;
}

void k_sprite_renderer_set_loop_callback(struct k_sprite_renderer *renderer, void (*fn_callback)(struct k_object *object)) {
    renderer->fn_loop_callback = fn_callback;
}

/* endregion */