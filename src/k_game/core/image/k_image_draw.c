#include "k_log.h"

#include "./_internal.h"

int k_image_draw(struct k_image *image, const struct k_int_rect *src_rect, const struct k_float_rect *dst_rect, struct k_image_draw_options *options) {

    if (NULL == image || NULL == dst_rect)
        return -1;

    if (dst_rect->w <= 0 || dst_rect->h <= 0)
        return 0;

    /* TODO 检查输出的矩形有没有在视野范围内
     */

    SDL_Rect src;
    if (NULL == src_rect) {
        src.x = 0;
        src.y = 0;
        src.w = image->image_w;
        src.h = image->image_h;
    }
    else {
        if (src_rect->w <= 0|| src_rect->h <= 0)
            return -1;

        src.x = src_rect->x;
        src.y = src_rect->y;
        src.w = src_rect->w;
        src.h = src_rect->h;
    }

    SDL_FRect dst;
    dst.x = dst_rect->x;
    dst.y = dst_rect->y;
    dst.w = dst_rect->w;
    dst.h = dst_rect->h;

    if (NULL == options) {
        if (0 != SDL_RenderCopyF(k__window.renderer, image->texture, &src, &dst)) {
            k_log_error("Failed to draw image, SDL error: %s", SDL_GetError());
            return -1;
        }
    } else {
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
    }

    return 0;
}
