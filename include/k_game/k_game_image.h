#ifndef K_GAME_IMAGE_H
#define K_GAME_IMAGE_H

#include "./k_game_rect.h"

/**
 * TODO docs
 */
struct k_image;

/* region [image_load] */

struct k_image *k_image_load(const char *filepath);

int k_image_set_name(struct k_image *image, const char *image_name);

struct k_image *k_image_find(const char *image_name);

/* endregion */

/* region [image_get] */

int k_image_get_width(struct k_image *image);

int k_image_get_height(struct k_image *image);

/* endregion */

/* region [image_draw] */

int k_image_draw(struct k_image *image, const struct k_int_rect *src_rect, float dst_x, float dst_y);

/* endregion */

#endif
