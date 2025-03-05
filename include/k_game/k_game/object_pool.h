#ifndef K_GAME__OBJECT_POOL_H
#define K_GAME__OBJECT_POOL_H

#include "k_list.h"

struct k_object;

/* region [room->object_pool] */

struct k_object_pool {

    struct k_list object_list;
};

struct k_object_pool_node {

    struct k_list_node iter_node;
};

void k__object_pool_init(struct k_object_pool *pool);

void k__object_pool_cleanup(struct k_object_pool *pool);

void k__object_pool_add(struct k_object_pool *pool, struct k_object *object);

void k__object_pool_del(struct k_object *object);

/* endregion */

#endif
