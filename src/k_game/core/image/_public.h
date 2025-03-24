#ifndef K_IMAGE_PUBLIC_H
#define K_IMAGE_PUBLIC_H

#include "SDL_render.h"

#include "../asset/k_asset_registry.h"

int k__init_image_registry(void);

void k__cleanup_image_registry(void);

struct k_image {

    struct k_asset_registry_node registry_node;

    int image_w;
    int image_h;

    SDL_Texture *texture;
};

#endif
