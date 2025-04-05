#ifndef K_POSITION_INTERNAL_H
#define K_POSITION_INTERNAL_H

#include "k_list.h"

#include "k_game.h"
#include "./_shared.h"

struct k_position {

    struct k_list_node child_list_node;

    struct k_position *parent;

    struct k_list child_list;

    struct k_component *component;

    float local_x;
    float local_y;

    float *world_x;
    float *world_y;
};

#endif
