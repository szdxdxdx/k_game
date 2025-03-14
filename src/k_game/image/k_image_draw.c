#include "k_log.h"

#include "../k_SDL/k_SDL.h"

#include "./k_image.h"

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
    dst.w = (float)src.w;
    dst.h = (float)src.h;

    if (0 != SDL_RenderCopyF(k__window.renderer, image->texture, &src, &dst)) {
        k_log_error("SDL_RenderCopy() failed: %s", SDL_GetError());
        return -1;
    }

    return 0;
}

int k_image_draw(struct k_image *image, const struct k_int_rect *src_rect, float dst_x, float dst_y) {
    /* TODO: assert( NULL != image ) */
    /* TODO: assert currently is in draw callback */

    if (NULL == image) {
        k_log_error("Failed to draw image. Image is NULL");
        return -1;
    }

    return k__image_draw(image, src_rect, dst_x, dst_y);
}
