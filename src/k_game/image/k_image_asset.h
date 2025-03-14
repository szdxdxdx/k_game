#ifndef K_GAME__IMAGE_ASSET_H
#define K_GAME__IMAGE_ASSET_H

#include "SDL_render.h"

#include "../asset/k_asset_registry.h"

/* region [image_registry] */

int k__image_registry_init(void);

void k__image_registry_cleanup(void);

/* endregion */

struct k_image {

    struct k_asset_registry_node registry_node;

    int w, h;

    SDL_Texture *texture;
};

struct k_image *k__image_create(SDL_Texture *texture);

void k__image_destroy(struct k_image *image);

#endif
