#ifndef K_GAME_IMAGE_H
#define K_GAME_IMAGE_H

#include "./k_game_rect.h"

struct k_image;

struct k_image *k_load_image(const char *image_name, const char *filepath);

int k_image_get_width(struct k_image *image);

int k_image_get_height(struct k_image *image);

int k_draw_image(const struct k_image *image, const struct k_int_rect *src_rect, int dst_x, int dst_y);

#endif
