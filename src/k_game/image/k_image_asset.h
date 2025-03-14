#ifndef K_GAME__IMAGE_ASSET_H
#define K_GAME__IMAGE_ASSET_H

#include "SDL_render.h"

#include "k_game_rect.h"

#include "../asset/k_asset_registry.h"

/* region [image_registry] */

int k__image_registry_init(void);

void k__image_registry_cleanup(void);

/* endregion */

struct k_image {

    struct k_asset_registry_node registry_node;

    /* [?] ref_count && is_deleted */

    int w, h;

    SDL_Texture *texture;
};

void k_image_release(struct k_image *image);

int k__image_draw(struct k_image *image, const struct k_int_rect *src_rect, float dst_x, float dst_y);

#endif
