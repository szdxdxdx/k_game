#include "SDL_image.h"

#include "k_log.h"

#include "k_game_alloc.h"

#include "../k_SDL/k_SDL.h"

#include "./k_image.h"

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
