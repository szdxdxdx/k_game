#include "./_internal.h"

/* region [renderer_speed] */

void k_sprite_renderer_set_speed(struct k_sprite_renderer *renderer, float speed) {

    if (NULL == renderer->sprite)
        return;

    if (speed <= 0.0f)
        renderer->speed = 0.0f;
    else
        renderer->speed = speed;
}

void k_sprite_renderer_add_speed(struct k_sprite_renderer *renderer, float speed_delta) {
    k_sprite_renderer_set_speed(renderer, renderer->speed + speed_delta);
}

float k_sprite_renderer_get_speed(struct k_sprite_renderer *renderer) {

    if (NULL == renderer->sprite)
        return 0;
    else
        return renderer->speed;
}

/* endregion */

/* region [renderer_draw] */

static void calc_frame_idx(struct k_sprite_renderer *renderer) {
    struct k_sprite *sprite = renderer->sprite;

    size_t frames_num = k_sprite_get_frames_num(sprite);

    renderer->timer += k_get_step_delta() * renderer->speed;
    uint64_t timer_ms = (uint64_t)(renderer->timer * 1000);
    while (1) {
        int delay = k_sprite_get_frame_delay(sprite, renderer->frame_idx);
        if (timer_ms < delay)
            break;

        timer_ms -= delay;
        renderer->frame_idx += 1;
        if (frames_num <= renderer->frame_idx)
            renderer->frame_idx = 0;
    }

    renderer->timer = (float)timer_ms / 1000;
}

static void draw_sprite(struct k_sprite_renderer *renderer) {

    if (transform_none == renderer->transform_flags) {
        k_sprite_draw(renderer->sprite, renderer->frame_idx, *(renderer->x), *(renderer->y), NULL);
    }
    else {
        struct k_sprite_draw_options opts;
        opts.scaled_w = renderer->scaled_w;
        opts.scaled_h = renderer->scaled_h;
        opts.angle    = renderer->angle;
        opts.flip_x   = renderer->transform_flags & transform_flip_x;
        opts.flip_y   = renderer->transform_flags & transform_flip_y;

        k_sprite_draw(renderer->sprite, renderer->frame_idx, *(renderer->x), *(renderer->y), &opts);
    }
}

static void draw_animation(struct k_component *component) {
    struct k_sprite_renderer *renderer = k_component_get_data(component);

    calc_frame_idx(renderer);
    draw_sprite(renderer);
}

static void draw_image(struct k_component *component) {
    struct k_sprite_renderer *renderer = k_component_get_data(component);

    draw_sprite(renderer);
}

/* endregion */

/* region [renderer_sprite] */

int k_sprite_renderer_set_sprite(struct k_sprite_renderer *renderer, struct k_sprite *sprite) {

    if (NULL == sprite) {

        if (NULL == renderer->sprite)
            return 0;

        k_component_del_callback(renderer->callback_draw_sprite);
        renderer->callback_draw_sprite = NULL;
    }
    else {
        struct k_component_callback *draw_callback;
        if (k_sprite_get_frames_num(sprite) == 1) {
            draw_callback = k_component_add_draw_callback(renderer->component, draw_image, renderer->z_index);
        } else {
            draw_callback = k_component_add_draw_callback(renderer->component, draw_animation, renderer->z_index);
        }

        if (NULL == draw_callback)
            return -1;

        k_component_del_callback(renderer->callback_draw_sprite);
        renderer->callback_draw_sprite = draw_callback;
    }

    renderer->sprite = sprite;

    renderer->timer = 0.0f;
    renderer->speed = 1.0f;
    renderer->frame_idx = 0;
    k_sprite_renderer_clear_transforms(renderer);

    k_sprite_renderer_set_debug(renderer, renderer->debug);
    return 0;
}

struct k_sprite *k_sprite_renderer_get_sprite(struct k_sprite_renderer *renderer) {
    return renderer->sprite;
}

/* endregion */

/* region [renderer_z_index] */

int k_sprite_renderer_set_z_index(struct k_sprite_renderer *renderer, int z_index) {

    if (NULL == renderer->sprite) {
        renderer->z_index = z_index;
        return 0;
    }

    struct k_component_callback *draw_callback;
    if (k_sprite_get_frames_num(renderer->sprite) == 1) {
        draw_callback = k_component_add_draw_callback(renderer->component, draw_image, z_index);
    } else {
        draw_callback = k_component_add_draw_callback(renderer->component, draw_animation, z_index);
    }

    if (NULL == draw_callback)
        return -1;

    k_component_del_callback(renderer->callback_draw_sprite);
    renderer->callback_draw_sprite = draw_callback;

    renderer->z_index = z_index;
    return 0;
}

int k_sprite_renderer_get_z_index(struct k_sprite_renderer *renderer) {
    return renderer->z_index;
}

/* endregion */
