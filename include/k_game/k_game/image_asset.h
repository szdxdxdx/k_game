#ifndef K_GAME__IMAGE_ASSET_H
#define K_GAME__IMAGE_ASSET_H

#include "SDL_render.h"

#include "k_game/asset_registry.h"

struct k_image {

    struct k_asset_registry_node registry_node;

    /* TODO ref_count && is_deleted */

    int w, h;

    SDL_Texture *texture;
};

void k__image_release(struct k_image *image);

#endif
