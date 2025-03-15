#ifndef K_GAME_IMAGE_H
#define K_GAME_IMAGE_H

#include <stdint.h>

#include "./k_game_rect.h"

/**
 * TODO docs
 */
struct k_image;

/* region [image_load] */

struct k_image *k_image_load(const char *filepath);

int k_image_set_name(struct k_image *image, const char *image_name);

struct k_image *k_image_find(const char *image_name);

/**
 * \brief 释放图片资源
 *
 * 释放图片资源前，请确保该图片没有被引用。
 * 若有精灵引用了该图片，释放图片后，精灵持有的图片指针将变为悬空指针。
 */
void k_image_release(struct k_image *image);

/* endregion */

/* region [image_get] */

int k_image_get_width(struct k_image *image);

int k_image_get_height(struct k_image *image);

/* endregion */

/* region [image_transform] */

struct k_image *k_image_scale(struct k_image *image, int scaled_w, int scaled_h);

/* endregion */

/* region [image_draw] */

int k_image_draw(struct k_image *image, const struct k_int_rect *src_rect, float dst_x, float dst_y);

struct k_image_draw_options {

    const struct k_int_rect *src_rect;

    float dst_x, dst_y;

    int dst_w, dst_h;

    float angle;
    float pivot_x, pivot_y;

    /* TODO alpha? */

    uint8_t horizontal_flip, vertical_flip;
};

int k_image_draw_ex(struct k_image *image, struct k_image_draw_options *options);

/* endregion */

#endif
