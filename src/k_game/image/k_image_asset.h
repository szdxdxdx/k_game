#ifndef K_GAME__IMAGE_ASSET_H
#define K_GAME__IMAGE_ASSET_H

#include "SDL_render.h"

#include "../asset/k_asset_registry.h"

struct k_image {

    struct k_asset_registry_node registry_node;

    /* [?] ref_count && is_deleted */

    int w, h;

    SDL_Texture *texture;
};

#endif
