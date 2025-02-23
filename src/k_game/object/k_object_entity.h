#ifndef K_GAME__OBJECT_ENTITY_H
#define K_GAME__OBJECT_ENTITY_H

#include "./k_object_pool.h"
#include "./k_object_callback.h"

struct k_object {

    struct k_object_pool_node object_node;

    struct k_object_callbacks_list callbacks_list;

    struct k_room *room;

    void *data;
};

#endif
