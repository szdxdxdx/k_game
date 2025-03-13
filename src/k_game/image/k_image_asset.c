#include "SDL_image.h"

#include "k_log.h"

#include "k_game_alloc.h"
#include "../k_SDL/k_SDL.h"
#include "./k_image.h"

/* region [image_load] */

struct k_image *k_image_load(const char *filepath) {

    if (NULL == filepath || '\0' == filepath[0])
        return NULL;

    struct k_image *image = k_malloc(sizeof(struct k_image));
    if (NULL == image)
        return NULL;

    image->texture = IMG_LoadTexture(k__window.renderer, filepath);
    if (NULL == image->texture) {
        k_log_error("IMG_LoadTexture() failed: %s", IMG_GetError());
        k_free(image);
        return NULL;
    }

    SDL_QueryTexture(image->texture, NULL, NULL, &image->w, &image->h);

    k__image_registry_add(image);
    return image;
}

void k__image_release(struct k_image *image) {
    k__image_registry_del(image);
    SDL_DestroyTexture(image->texture);
    k_free(image);
}

/* endregion */

/* region [image_get] */

int k_image_get_width(struct k_image *image) {
    return image->w;
}

int k_image_get_height(struct k_image *image) {
    return image->h;
}

/* endregion */

/* region [image_draw] */

int k__image_draw(struct k_image *image, const struct k_int_rect *src_rect, float dst_x, float dst_y) {

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

/* endregion */
