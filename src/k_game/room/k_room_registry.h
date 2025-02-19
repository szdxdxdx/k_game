#ifndef K_GAME__ROOM_REGISTRY_H
#define K_GAME__ROOM_REGISTRY_H

#include "k_list.h"
#include "k_str_map.h"

/* 房间注册表
 *
 */
struct k_room_registry {

    struct k_list rooms_list;

    struct k_str_map name_map;
};

/* 用于房间注册表的结点结构
 *
 */
struct k_room_registry_node {

    struct k_list_node iter_node;

    struct k_str_map_node name_map_node;
};

void k__room_registry_init(void);

void k__room_registry_deinit(void);

int k__room_registry_add(struct k_room_registry_node *node, const char *room_name);

void k__room_registry_del(struct k_room_registry_node *node);

#endif
