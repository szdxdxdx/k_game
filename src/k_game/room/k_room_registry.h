#ifndef K_GAME__ROOM_REGISTRY_H
#define K_GAME__ROOM_REGISTRY_H

#include "k_list.h"
#include "k_str_map.h"

struct k_room_registry_node {

    struct k_list_node iter_node;

    struct k_str_map_node name_map_node;
};

void k_room_registry_init(void);

void k_room_registry_deinit(void);

int k_room_registry_add(struct k_room_registry_node *node, const char *room_name);

void k_room_registry_del(struct k_room_registry_node *node);

#endif
