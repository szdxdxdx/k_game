#include "./_internal.h"

#include "../../core/sprite/_public.h"

/* region [renderer_draw] */

static void draw_sprite(struct k_component *component) {
    struct k_sprite_renderer *renderer = k_component_get_data(component);

    renderer->timer += k_get_step_delta() * renderer->speed;

    int is_loop = 0;

    struct k_sprite *sprite = renderer->sprite;
    size_t frames_num = k_sprite_get_frames_num(sprite);
    while (1) {
        float delay = (float)k_sprite_get_frame_delay(sprite, renderer->frame_idx) / 1000;
        if (renderer->timer < delay)
            break;

        renderer->timer -= delay;
        renderer->frame_idx += 1;
        if (frames_num <= renderer->frame_idx) {

            if (0 < renderer->loop) {
                renderer->loop -= 1;
                is_loop = 1;

                if (1 < renderer->loop) {
                    renderer->frame_idx = 0;
                } else {
                    renderer->frame_idx = frames_num - 1;
                }
            }
        }
    }

    if (transform_none == renderer->transform_flags) {
        k_sprite_draw(sprite, renderer->frame_idx, *(renderer->x), *(renderer->y), NULL);
    }
    else {
        struct k_sprite_draw_options opts;
        opts.scaled_w = renderer->scaled_w;
        opts.scaled_h = renderer->scaled_h;
        opts.angle    = renderer->angle;
        opts.flip_x   = renderer->transform_flags & transform_flip_x;
        opts.flip_y   = renderer->transform_flags & transform_flip_y;

        k_sprite_draw(sprite, renderer->frame_idx, *(renderer->x), *(renderer->y), &opts);
    }

    if (is_loop && NULL != renderer->fn_loop_callback)
        renderer->fn_loop_callback(renderer->loop_callback_data);
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
        struct k_component_callback *callback = k_component_add_draw_callback(renderer->component, draw_sprite, renderer->z_index);
        if (NULL == callback)
            return -1;

        k_component_del_callback(renderer->callback_draw_sprite);
        renderer->callback_draw_sprite = callback;
    }

    renderer->sprite = sprite;

    renderer->frame_idx = 0;
    renderer->timer = 0;
    renderer->speed = 1;
    k_sprite_renderer_reset_transforms(renderer);

    k_sprite_renderer_set_debug(renderer, renderer->debug);
    return 0;
}

struct k_sprite *k_sprite_renderer_get_sprite(struct k_sprite_renderer *renderer) {
    return renderer->sprite;
}

/* endregion */

/* region [z_index] */

int k_sprite_renderer_set_z_index(struct k_sprite_renderer *renderer, int z_index) {

    if (NULL == renderer->sprite) {
        renderer->z_index = z_index;
        return 0;
    }

    struct k_component_callback *callback = k_component_add_draw_callback(renderer->component, draw_sprite, z_index);
    if (NULL == callback)
        return -1;

    k_component_del_callback(renderer->callback_draw_sprite);
    renderer->callback_draw_sprite = callback;

    renderer->z_index = z_index;
    return 0;
}

int k_sprite_renderer_get_z_index(struct k_sprite_renderer *renderer) {
    return renderer->z_index;
}

/* endregion */

/* region [speed] */

void k_sprite_renderer_set_speed(struct k_sprite_renderer *renderer, float speed) {

    if (NULL == renderer->sprite)
        return;

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

    if (loop < 0)
        renderer->loop = 0;
    else
        renderer->loop = loop;
}

void k_sprite_renderer_set_loop_callback(struct k_sprite_renderer *renderer, void (*fn_callback)(void *data), void *data) {
    renderer->fn_loop_callback = fn_callback;
    renderer->loop_callback_data = data;
}

/* endregion */