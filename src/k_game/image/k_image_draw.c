#include "SDL.h"
#include "SDL_image.h"

#include "k_log.h"

#include "k_game/image.h"
#include "../game/k_game.h"
#include "./k_image.h"

int k_draw_image(struct k_image *image, const struct k_int_rect *rect, int x, int y) {

    if (NULL == image) {
        k_log_error_once("Image is NULL");
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

    if (0 != SDL_RenderCopy(k_game.renderer, image->texture, &src, &dst)) {
        k_log_error("SDL_RenderCopy() failed: %s", IMG_GetError());
        return -1;
    }

    return 0;
}
