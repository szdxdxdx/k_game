#ifndef K_GAME__OBJECT_ENTITY_H
#define K_GAME__OBJECT_ENTITY_H

#include "k_list.h"

#include "object_pool.h"

struct k_object {

    struct k_object_pool_node pool_node;

    struct k_room *room;

    struct k_list callback_list;

    struct k_list component_list;

    void *data;
};

#endif
