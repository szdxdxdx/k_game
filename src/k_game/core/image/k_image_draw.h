#ifndef K_GAME__IMAGE_DRAW_H
#define K_GAME__IMAGE_DRAW_H

struct k_int_rect;
struct k_image;

int k__image_draw(struct k_image *image, const struct k_int_rect *src_rect, float dst_x, float dst_y);

int k__image_draw_ex(struct k_image *image, struct k_image_draw_options *options);

#endif
