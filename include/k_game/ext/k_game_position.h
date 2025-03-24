#ifndef K_GAME_POSITION_H
#define K_GAME_POSITION_H

#include "k_list.h"

#include "../core/k_game_rect.h"

struct k_position {
    struct k_list_node list_node;
    struct k_list list;

    struct k_position  *parent;
    struct k_float_vec2 offset;
    struct k_float_vec2 position;
};

void k_position_init(struct k_position *self, struct k_position *parent, float offset_x, float offset_y);

void k_position_set_offset(struct k_position *self, float x, float y);

#endif
