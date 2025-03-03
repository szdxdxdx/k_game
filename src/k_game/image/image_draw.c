#include "k_log.h"

#include "k_game/k_SDL.h"
#include "k_game/game_context.h"
#include "k_game_image.h"
#include "k_game/image_asset.h"
#include "k_game/image_draw.h"

int k_draw_image(const struct k_image *image, const struct k_int_rect *src_rect, int dst_x, int dst_y) {
    /* TODO: assert( NULL != image ) */
    /* TODO: assert currently is in draw callback */

    if (NULL == image) {
        k_log_error_once("Failed to draw image. Image is NULL");
        return -1;
    }

    return k__draw_image(image, src_rect, dst_x, dst_y);
}

int k__draw_image(const struct k_image *image, const struct k_int_rect *src_rect, int dst_x, int dst_y) {

    SDL_Rect src;
    if (NULL == src_rect) {
        src.x = 0;
        src.y = 0;
        src.w = image->w;
        src.h = image->h;
    } else {
        src.x = src_rect->x;
        src.y = src_rect->y;
        src.w = src_rect->w;
        src.h = src_rect->h;
    }

    SDL_Rect dst;
    dst.x = dst_x;
    dst.y = dst_y;
    dst.w = src.w;
    dst.h = src.h;

    if (0 != SDL_RenderCopy(k__window.renderer, image->texture, &src, &dst)) {
        k_log_error("SDL_RenderCopy() failed: %s", SDL_GetError());
        return -1;
    }

    return 0;
}
