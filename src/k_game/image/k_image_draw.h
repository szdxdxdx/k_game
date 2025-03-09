#ifndef K_GAME__IMAGE_DRAW_H
#define K_GAME__IMAGE_DRAW_H

#include "k_game_rect.h"

struct k_image;

int k__image_draw(struct k_image *image, const struct k_int_rect *src_rect, int dst_x, int dst_y);

#endif
