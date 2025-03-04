#ifndef K_GAME__IMAGE_ASSET_H
#define K_GAME__IMAGE_ASSET_H

#include "SDL.h"

#include "k_list.h"

#include "k_game/asset_registry.h"

struct k_image;

/* region [image_registry] */

int k__image_registry_init(void);

void k__image_registry_cleanup(void);

int k__image_registry_add(struct k_image *image, const char *image_name);

void k__image_registry_del(struct k_image *image);

/* endregion */

/* region [image] */

struct k_image {

    struct k_asset_registry_node registry_node;

    /* TODO ref_count && is_deleted */

    int w, h;

    SDL_Texture *texture;
};

void k__image_release(struct k_image *image);

const char *k__image_get_name(struct k_image *image);

/* endregion */

#endif
