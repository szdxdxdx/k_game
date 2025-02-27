#ifndef K_GAME__OBJECT_POOL_H
#define K_GAME__OBJECT_POOL_H

#include "k_list.h"

struct k_room;
struct k_object;

struct k_object_pool {

    struct k_list objects_list;
};

struct k_object_pool_node {

    struct k_list_node iter_node;
};

void k__room_init_object_pool(struct k_room *room);

void k__room_deinit_object_pool(struct k_room *room);

/* 从 object pool 中取出一个 object，且取出的 object 已和 room 建立关联
 * 若成功，函数返回 object 的指针，否则返回 `NULL`。
 */
struct k_object *k__room_object_pool_alloc(struct k_room *room);

void k__room_object_pool_free(struct k_object *object);

#endif
