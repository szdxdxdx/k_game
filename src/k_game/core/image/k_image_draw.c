#include "k_log.h"

#include "../k_SDL/k_SDL.h"

#include "./k_image.h"

/* region [image_draw] */

int k__image_draw(struct k_image *image, const struct k_int_rect *src_rect, float dst_x, float dst_y) {

    SDL_Rect src;
    if (NULL == src_rect) {
        src.x = 0;
        src.y = 0;
        src.w = image->image_w;
        src.h = image->image_h;
    } else {
        src.x = src_rect->x;
        src.y = src_rect->y;
        src.w = src_rect->w;
        src.h = src_rect->h;
    }

    SDL_FRect dst;
    dst.x = dst_x;
    dst.y = dst_y;
    dst.w = (float)src.w; /* TODO (w > 0 && h > 0) */
    dst.h = (float)src.h;

    if (0 != SDL_RenderCopyF(k__window.renderer, image->texture, &src, &dst)) {
        k_log_error("Failed to draw image, SDL error: %s", SDL_GetError());
        return -1;
    }

    return 0;
}

int k_image_draw(struct k_image *image, const struct k_int_rect *src_rect, float dst_x, float dst_y) {

    if (NULL == image) {
        k_log_error("Failed to draw image. image is NULL");
        return -1;
    }

    /* [?] assert currently is in draw callback */

    return k__image_draw(image, src_rect, dst_x, dst_y);
}

/* endregion */

/* region [image_draw_ex] */

int k__image_draw_ex(struct k_image *image, struct k_image_draw_options *options) {

    const struct k_int_rect *src_rect = options->src_rect;

    SDL_Rect src;
    if (NULL == src_rect) {
        src.x = 0;
        src.y = 0;
        src.w = image->image_w;
        src.h = image->image_h;
    } else {
        src.x = src_rect->x;
        src.y = src_rect->y;
        src.w = src_rect->w;
        src.h = src_rect->h;
    }

    SDL_FRect dst;
    dst.x = options->dst_x;
    dst.y = options->dst_y;
    dst.w = (float)options->dst_w; /* TODO (w > 0 && h > 0) */
    dst.h = (float)options->dst_h;

    SDL_FPoint center;
    center.x = options->pivot_x;
    center.y = options->pivot_y;

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (options->flip_x)
        flip |= SDL_FLIP_HORIZONTAL;
    if (options->flip_y)
        flip |= SDL_FLIP_VERTICAL;

    if (0 != SDL_RenderCopyExF(k__window.renderer, image->texture, &src, &dst, options->angle, &center, flip)) {
        k_log_error("Failed to draw image, SDL error: %s", SDL_GetError());
        return -1;
    }

    return 0;
}

int k_image_draw_ex(struct k_image *image, struct k_image_draw_options *options) {

    if (NULL == image) {
        k_log_error("Failed to draw image. image is NULL");
        return -1;
    }

    if (NULL == options) {
        k_log_error("Failed to draw image. options is NULL");
        return -1;
    }

    /* [?] assert currently is in draw callback */

    return k__image_draw_ex(image, options);
}

/* endregion */
