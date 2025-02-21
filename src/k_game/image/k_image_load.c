#include "SDL_image.h"

#include "k_log.h"

#include "k_game/alloc.h"
#include "k_game/image.h"
#include "../game/k_game_context.h"
#include "../SDL/k_SDL_window.h"
#include "./k_image_load.h"

struct k_image *k_load_image(const struct k_image_config *config) {

    struct k_image *img = k_malloc(sizeof(struct k_image));
    if (NULL == img)
        goto malloc_failed;

    img->texture = IMG_LoadTexture(k__window.renderer, config->filepath);
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
