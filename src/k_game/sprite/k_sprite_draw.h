#ifndef K_GAME__SPRITE_DRAW_H
#define K_GAME__SPRITE_DRAW_H

#include <stddef.h>

struct k_sprite;

int k__draw_sprite_frame(struct k_sprite *sprite, int x, int y, size_t frame_idx);

#endif
