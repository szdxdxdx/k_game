#include "k_log.h"

#include "k_game/image.h"
#include "../game/k_game_context.h"
#include "../SDL/k_SDL_window.h"
#include "./k_image_load.h"

int k_draw_image(struct k_image *image, const struct k_int_rect *rect, int x, int y) {
    /* TODO: assert( NULL != image ) */
    /* TODO: assert currently is in draw callback */

    if (NULL == image) {
        k_log_error_once("Failed to draw image. Image is NULL");
        return -1;
    }

    SDL_Rect src;
    if (NULL == rect) {
        src.x = 0;
        src.y = 0;
        src.w = image->w;
        src.h = image->h;
    } else {
        src.x = rect->x;
        src.y = rect->y;
        src.w = rect->w;
        src.h = rect->h;
    }

    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = src.w;
    dst.h = src.h;

    if (0 != SDL_RenderCopy(k__window.renderer, image->texture, &src, &dst)) {
        k_log_error("SDL_RenderCopy() failed: %s", SDL_GetError());
        return -1;
    }

    return 0;
}
