#include "SDL.h"
#include "SDL_image.h"

#include "k_log.h"

#include "k_game/alloc.h"
#include "k_game/image.h"
#include "../game/k_game_context.h"
#include "./k_image.h"

struct k_image *k_load_image(const struct k_image_config *config) {

    struct k_image *img = k_malloc(sizeof(struct k_image));
    if (NULL == img)
        goto malloc_failed;

    img->texture = IMG_LoadTexture(k_game.renderer, config->filepath);
    if (NULL == img->texture) {
        k_log_error("IMG_LoadTexture() failed: %s", IMG_GetError());
        goto load_img_failed;
    }

    if (0 != SDL_QueryTexture(img->texture, NULL, NULL, &img->w, &img->h)) {
        k_log_error("SDL_QueryTexture() failed: %s", IMG_GetError());
        goto query_img_failed;
    }

    return img;

query_img_failed:
load_img_failed:
    k_free(img);

malloc_failed:
    return NULL;
}

int k_image_get_width(struct k_image *image) {
    return image->w;
}

int k_image_get_height(struct k_image *image) {
    return image->h;
}

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
