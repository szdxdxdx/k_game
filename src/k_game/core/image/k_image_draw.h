#ifndef K_GAME__IMAGE_DRAW_H
#define K_GAME__IMAGE_DRAW_H

struct k_int_rect;
struct k_image;

int k__image_draw(struct k_image *image, const struct k_int_rect *src_rect, float dst_x, float dst_y);

#endif
