#ifndef K_GAME_POSITION_H
#define K_GAME_POSITION_H

#include "k_list.h"

#include "../core/k_game_rect.h"

struct k_position {
    struct k_list_node list_node;
    struct k_list list;

    float x;
    float y;

    struct k_position *parent;
    float rel_x;
    float rel_y;
};

void k_position_init(struct k_position *self, struct k_position *parent, float rel_x, float rel_y);

void k_position_set(struct k_position *self, float rel_x, float rel_y);

#endif
