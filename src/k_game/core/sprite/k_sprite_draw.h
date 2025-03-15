#ifndef K_GAME__SPRITE_DRAW_H
#define K_GAME__SPRITE_DRAW_H

#include <stddef.h>

struct k_sprite;

int k__sprite_draw_frame(struct k_sprite *sprite, size_t frame_idx, float dst_x, float dst_y);

#endif
