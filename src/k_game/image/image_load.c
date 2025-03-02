#include "SDL_image.h"

#include "k_log.h"

#include "k_game_alloc.h"
#include "k_game/k_SDL.h"
#include "k_game_image.h"
#include "k_game/image_load.h"

struct k_image *k_load_image(const char *image_name, const char *filepath) {

    struct k_image *img = k_malloc(sizeof(struct k_image));
    if (NULL == img)
        goto malloc_failed;

    img->texture = IMG_LoadTexture(k__window.renderer, filepath);
    if (NULL == img->texture) {
        k_log_error("IMG_LoadTexture() failed: %s", IMG_GetError());
        goto load_failed;
    }

    if (0 != SDL_QueryTexture(img->texture, NULL, NULL, &img->w, &img->h)) {
        k_log_error("SDL_QueryTexture() failed: %s", IMG_GetError());
        goto query_failed;
    }

    return img;

query_failed:
    SDL_DestroyTexture(img->texture);

load_failed:
    k_free(img);

malloc_failed:
    k_log_error("Failed to load image");
    return NULL;
}

int k_image_get_width(struct k_image *image) {
    return image->w;
}

int k_image_get_height(struct k_image *image) {
    return image->h;
}
