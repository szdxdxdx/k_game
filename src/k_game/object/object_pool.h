#ifndef K_GAME__OBJECT_POOL_H
#define K_GAME__OBJECT_POOL_H

#include "k_list.h"

struct k_object;

struct k_object_pool {

    struct k_list object_list;
};

struct k_object_pool_node {

    struct k_list_node iter_node;
};

int k__object_pool_init(struct k_object_pool *pool);

void k__object_pool_cleanup(struct k_object_pool *pool);

struct k_object *k__object_pool_acquire(struct k_object_pool *pool);

void k__object_pool_release(struct k_object *object);

#endif
