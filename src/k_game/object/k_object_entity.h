#ifndef K_GAME__OBJECT_ENTITY_H
#define K_GAME__OBJECT_ENTITY_H

#include "./k_object_pool.h"
#include "./k_object_callback.h"
#include "../component/k_component_create.h"

struct k_object {

    struct k_object_pool_node object_node;

    struct k_room *room;

    struct k_object_callbacks_list callbacks;

    struct k_object_component_list components;

    void *data;
};

#endif
