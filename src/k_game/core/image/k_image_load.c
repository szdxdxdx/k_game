#include "SDL_image.h"

#include "k_game/core/k_image.h"
#include "./k_image_create.h"

#include "../k_SDL/k_window.h"

struct k_image *k_image_load(const char *filepath) {

    if (NULL == filepath || '\0' == filepath[0])
        goto err;

    SDL_Texture *texture = IMG_LoadTexture(k__window.renderer, filepath);
    if (NULL == texture)
        goto err;

    struct k_image *image = k__image_create(texture);
    if (NULL == image) {
        SDL_DestroyTexture(texture);
        goto err;
    }

    return image;

err:
    return NULL;
}

void k_image_release(struct k_image *image) {

    if (NULL != image)
        k__image_destroy(image);
}
