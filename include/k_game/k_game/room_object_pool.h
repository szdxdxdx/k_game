#ifndef K_GAME__OBJECT_POOL_H
#define K_GAME__OBJECT_POOL_H

#include "k_list.h"

struct k_room;
struct k_object;

/* region [room->object_pool] */

struct k_object_pool {

    struct k_list object_list;
};

struct k_object_pool_node {

    struct k_list_node iter_node;
};

void k__room_init_object_pool(struct k_room *room);

void k__room_cleanup_object_pool(struct k_room *room);

struct k_object *k__room_object_pool_acquire(struct k_room *room);

void k__room_object_pool_release(struct k_object *object);

/* endregion */

#endif
