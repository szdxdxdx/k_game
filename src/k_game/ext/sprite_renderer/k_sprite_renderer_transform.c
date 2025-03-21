#include "./_internal.h"

/* region [scale] */

void k_sprite_renderer_set_w(struct k_sprite_renderer *renderer, float scaled_w) {

    if (NULL == renderer->sprite)
        return;

    if (scaled_w <= 0) {
        renderer->scaled_w = 0;
    } else {
        renderer->scaled_w = scaled_w;

        if (k_sprite_get_width(renderer->sprite) != (int)scaled_w)
            renderer->transform_flags |= transform_scale_x;
        else
            renderer->transform_flags &= ~transform_scale_x;
    }
}

void k_sprite_renderer_scale_x(struct k_sprite_renderer *renderer, float scale_x) {

    if (NULL == renderer->sprite)
        return;

    if (scale_x <= 0) {
        renderer->scaled_w = 0;
        renderer->transform_flags |= transform_scale_x;
    }
    else {
        float sprite_w = (float)k_sprite_get_width(renderer->sprite);

        renderer->scaled_w = sprite_w * scale_x;

        if (sprite_w != renderer->scaled_w)
            renderer->transform_flags |= transform_scale_x;
        else
            renderer->transform_flags &= ~transform_scale_x;
    }
}

float k_sprite_renderer_get_w(struct k_sprite_renderer *renderer) {

    if (NULL == renderer->sprite)
        return 0;
    else
        return renderer->scaled_w;
}

/* endregion */

/* region [scale_y] */

void k_sprite_renderer_set_h(struct k_sprite_renderer *renderer, float scaled_h) {

    if (NULL == renderer->sprite)
        return;

    if (scaled_h <= 0) {
        renderer->scaled_h = 0;
    } else {
        renderer->scaled_h = scaled_h;

        if (k_sprite_get_height(renderer->sprite) != (int)scaled_h)
            renderer->transform_flags |= transform_scale_y;
        else
            renderer->transform_flags &= ~transform_scale_y;
    }
}

void k_sprite_renderer_scale_y(struct k_sprite_renderer *renderer, float scale_y) {

    if (NULL == renderer->sprite)
        return;

    if (scale_y <= 0) {
        renderer->scaled_h = 0;
        renderer->transform_flags |= transform_scale_y;
    }
    else {
        float sprite_h = (float)k_sprite_get_height(renderer->sprite);

        renderer->scaled_h = sprite_h * scale_y;

        if (sprite_h != renderer->scaled_h)
            renderer->transform_flags |= transform_scale_y;
        else
            renderer->transform_flags &= ~transform_scale_y;
    }
}

float k_sprite_renderer_get_h(struct k_sprite_renderer *renderer) {

    if (NULL == renderer->sprite)
        return 0;
    else
        return renderer->scaled_h;
}

/* endregion */

/* region [rotate] */

void k_sprite_renderer_rotate(struct k_sprite_renderer *renderer, float angle) {

    renderer->angle = angle;
    if (0 != angle)
        renderer->transform_flags |= transform_rotate;
    else
        renderer->transform_flags &= ~transform_rotate;
}

float k_sprite_renderer_get_rotation(struct k_sprite_renderer *renderer) {

    if (NULL == renderer->sprite)
        return 0;
    else
        return renderer->angle;
}

/* endregion */

/* region [flip_x] */

void k_sprite_renderer_flip_x(struct k_sprite_renderer *renderer, int flip) {

    if (flip)
        renderer->transform_flags |= transform_flip_x;
    else
        renderer->transform_flags &= ~transform_flip_x;
}

int k_sprite_renderer_is_flipped_x(struct k_sprite_renderer *renderer) {

    if (NULL == renderer->sprite)
        return 0;
    else
        return renderer->transform_flags & transform_flip_x;
}

/* endregion */

/* region [flip_y] */

void k_sprite_renderer_flip_y(struct k_sprite_renderer *renderer, int flip) {

    if (flip)
        renderer->transform_flags |= transform_flip_y;
    else
        renderer->transform_flags &= ~transform_flip_y;
}

int k_sprite_renderer_is_flipped_y(struct k_sprite_renderer *renderer) {

    if (NULL == renderer->sprite)
        return 0;
    else
        return renderer->transform_flags & transform_flip_y;
}

/* endregion */

void k_sprite_renderer_reset_transforms(struct k_sprite_renderer *renderer) {

    if (NULL != renderer->sprite) {
        renderer->scaled_w = (float)k_sprite_get_width(renderer->sprite);
        renderer->scaled_h = (float)k_sprite_get_height(renderer->sprite);
    } else {
        renderer->scaled_w = 0;
        renderer->scaled_h = 0;
    }

    renderer->angle = 0;
    renderer->transform_flags = transform_none;
}
