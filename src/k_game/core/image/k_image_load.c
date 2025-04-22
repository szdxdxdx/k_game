#include "SDL_image.h"

#define K_LOG_TAG "k_game:image"
#include "k_log.h"

#include "k_game/core/k_image.h"
#include "./k_image_create.h"

#include "../k_SDL/k_window.h"

struct k_image *k_image_load(const char *file_path) {

    if (NULL == file_path || '\0' == file_path[0])
        return NULL;

    SDL_Texture *texture = IMG_LoadTexture(k__window.renderer, file_path);
    if (NULL == texture) {
        k_log_error("SDL error: %s", IMG_GetError());
        return NULL;
    }

    struct k_image *image = k__image_create(texture);
    if (NULL == image) {
        SDL_DestroyTexture(texture);
        return NULL;
    }

    return image;
}

void k_image_release(struct k_image *image) {

    if (NULL == image)
        return;

    k__image_destroy(image);
}
