#ifndef K_GAME__POSITION__INTERNAL_H
#define K_GAME__POSITION__INTERNAL_H

#include "k_list.h"

#include "k_game.h"
#include "./_public.h"

struct k_position {

    struct k_list_node list_node;

    struct k_component *component;

    float *x;
    float *y;

    struct k_position *parent;

    float rel_x;
    float rel_y;

    struct k_list list;
};

#endif
