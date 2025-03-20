#ifndef K_GAME__OBJECT__PUBLIC_H
#define K_GAME__OBJECT__PUBLIC_H

#include "k_list.h"

struct k_room;
struct k_object;

/* region [object_pool] */

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

/* endregion */

/* region [object] */

struct k_object {

    struct k_object_pool_node pool_node;

    struct k_room *room;

    struct k_list callback_list;

    struct k_list component_list;

    void *data;
};

/* endregion */

#endif
