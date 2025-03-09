#ifndef K_GAME__SPRITE_DRAW_H
#define K_GAME__SPRITE_DRAW_H

#include <stddef.h>

struct k_sprite;

int k__sprite_draw_frame(struct k_sprite *sprite, int dst_x, int dst_y, size_t frame_idx);

#endif
