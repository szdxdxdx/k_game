#ifndef K_IMAGE_H
#define K_IMAGE_H

#include <stdint.h>

#include "./k_game_fwd.h"

struct k_image *k_image_load(const char *file_path);

/**
 * \brief 释放图片资源
 *
 * 释放图片资源前，请确保该图片没有被引用。
 * 若有精灵引用了该图片，释放图片后，精灵持有的图片指针将变为悬空指针。
 */
void k_image_release(struct k_image *image);

int k_image_set_name(struct k_image *image, const char *image_name);

struct k_image *k_image_find(const char *image_name);

int k_image_get_width(struct k_image *image);

int k_image_get_height(struct k_image *image);

struct k_image *k_image_scale(struct k_image *image, int scaled_w, int scaled_h);

#endif
