#include "./k_sprite_renderer_internal.h"

/* region [scale_x] */

void k_sprite_renderer_set_scaled_w(struct k_sprite_renderer *renderer, float scaled_w) {

    if (scaled_w <= 0.0f) {
        renderer->scaled_w = 0.0f;
    } else {
        renderer->scaled_w = scaled_w;
    }

    renderer->transform_flags |= transform_scaled_w;
    renderer->transform_flags &= ~transform_scale_x;
}

void k_sprite_renderer_scale_x(struct k_sprite_renderer *renderer, float scale_x) {

    if (scale_x <= 0.0f) {
        renderer->scale_x = 0.0f;
    } else {
        renderer->scale_x = scale_x;
    }

    if (NULL != renderer->sprite) {
        float sprite_w = (float)k_sprite_get_w(renderer->sprite);
        renderer->scaled_w = sprite_w * scale_x;
    }

    renderer->transform_flags &= ~transform_scaled_w;

    if (1.0f == renderer->scale_x) {
        renderer->transform_flags &= ~transform_scale_x;
    } else {
        renderer->transform_flags |= transform_scale_x;
    }
}

float k_sprite_renderer_get_w(struct k_sprite_renderer *renderer) {
    return renderer->scaled_w;
}

/* endregion */

/* region [scale_y] */

void k_sprite_renderer_set_scaled_h(struct k_sprite_renderer *renderer, float scaled_h) {

    if (scaled_h <= 0.0f) {
        renderer->scaled_h = 0.0f;
    } else {
        renderer->scaled_h = scaled_h;
    }

    renderer->transform_flags |= transform_scaled_h;
    renderer->transform_flags &= ~transform_scale_y;
}

void k_sprite_renderer_scale_y(struct k_sprite_renderer *renderer, float scale_y) {

    if (scale_y <= 0.0f) {
        renderer->scale_y = 0.0f;
    } else {
        renderer->scale_y = scale_y;
    }

    if (NULL != renderer->sprite) {
        float sprite_h = (float)k_sprite_get_h(renderer->sprite);
        renderer->scaled_h = sprite_h * scale_y;
    }

    renderer->transform_flags &= ~transform_scaled_h;

    if (1.0f ==  renderer->scale_y) {
        renderer->transform_flags &= ~transform_scale_y;
    } else {
        renderer->transform_flags |= transform_scale_y;
    }
}

float k_sprite_renderer_get_h(struct k_sprite_renderer *renderer) {
    return renderer->scaled_h;
}

/* endregion */

/* region [rotate] */

void k_sprite_renderer_rotate(struct k_sprite_renderer *renderer, float angle) {

    renderer->angle = angle;
    if (0.0f != angle)
        renderer->transform_flags |= transform_rotate;
    else
        renderer->transform_flags &= ~transform_rotate;
}

float k_sprite_renderer_get_rotation(struct k_sprite_renderer *renderer) {
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
    return renderer->transform_flags & transform_flip_y;
}

/* endregion */

void k_sprite_renderer_reset_transforms(struct k_sprite_renderer *renderer) {

    if (NULL != renderer->sprite) {
        renderer->scaled_w = (float)k_sprite_get_w(renderer->sprite);
        renderer->scaled_h = (float)k_sprite_get_h(renderer->sprite);
    }

    renderer->scale_x = 1.0f;
    renderer->scale_y = 1.0f;
    renderer->angle = 0.0f;
    renderer->transform_flags = transform_none;
}
