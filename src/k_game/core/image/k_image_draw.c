#include "k_log.h"

#include "../k_SDL/k_SDL.h"

#include "./k_image.h"

int k_image_draw(struct k_image *image, struct k_image_draw_options *options) {

    if (NULL == image || NULL == options)
        return -1;

    SDL_FRect dst;
    dst.x = options->dst_x;
    dst.y = options->dst_y;
    dst.w = (float)options->scaled_w;
    dst.h = (float)options->scaled_h;

    if (dst.w <= 0 || dst.h <= 0)
        return 0;

    /* TODO 检查输出的矩形有没有在视野范围内
     */

    SDL_Rect src;
    if (NULL == options->src_rect) {
        src.x = 0;
        src.y = 0;
        src.w = image->image_w;
        src.h = image->image_h;
    } else {
        src.x = options->src_rect->x;
        src.y = options->src_rect->y;
        src.w = options->src_rect->w;
        src.h = options->src_rect->h;
    }

    if (src.w <= 0|| src.h <= 0)
        return -1;

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
