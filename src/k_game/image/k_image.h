#ifndef K_GAME__IMAGE_H
#define K_GAME__IMAGE_H

#include "SDL_render.h"

#include "k_game_image.h"

#include "../asset/k_asset_registry.h"

/* region [image_registry] */

int k__image_registry_init(void);

void k__image_registry_cleanup(void);

int k__image_registry_add(struct k_image *image, const char *image_name);

void k__image_registry_del(struct k_image *image);

const char *k__image_get_name(struct k_image *image);

/* endregion */

/* region [image] */

struct k_image {

    struct k_asset_registry_node registry_node;

    /* TODO ref_count && is_deleted */

    int w, h;

    SDL_Texture *texture;
};

void k__image_release(struct k_image *image);

/* endregion */

/* region [image_draw] */

struct k_int_rect;

int k__image_draw(const struct k_image *image, const struct k_int_rect *src_rect, int dst_x, int dst_y);

/* endregion */

#endif
