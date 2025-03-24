#ifndef K_POSITION_INTERNAL_H
#define K_POSITION_INTERNAL_H

#include "k_list.h"

#include "k_game.h"
#include "./_public.h"

struct k_position {

    struct k_list_node list_node;

    struct k_component *component;

    float *world_x;
    float *world_y;

    struct k_position *parent;

    float local_x;
    float local_y;

    struct k_list list;
};

#endif
