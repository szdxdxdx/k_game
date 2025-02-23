#ifndef K_GAME__OBJECT_POOL_H
#define K_GAME__OBJECT_POOL_H

#include "k_list.h"

struct k_object_pool {

    struct k_list objects_list;
};

struct k_object_pool_node {

    struct k_list_node iter_node;
};

void k__init_object_pool(struct k_object_pool *object_pool);

void k__deinit_object_pool(struct k_object_pool *object_pool);

#endif
