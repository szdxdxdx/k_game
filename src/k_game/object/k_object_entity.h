#ifndef K_GAME__OBJECT_ENTITY_H
#define K_GAME__OBJECT_ENTITY_H

#include "k_list.h"

#include "./k_object_pool.h"

struct k_room;

struct k_object {

    struct k_object_pool_node pool_node;

    struct k_room *room;

    struct k_list callbacks;

    struct k_list components;

    void *data;
};

#endif
