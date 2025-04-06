#ifndef K__IMAGE_SHARED_H
#define K__IMAGE_SHARED_H

#include "SDL_render.h"

#include "../asset/k_asset_registry.h"

int k__image_registry_init(void);

void k__image_registry_cleanup(void);

struct k_image {

    struct k_asset_registry_node registry_node;

    int image_w;
    int image_h;

    SDL_Texture *texture;
};

#endif
