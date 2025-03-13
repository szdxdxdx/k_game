#ifndef K_GAME__COMPONENT_MANAGER_MAP_H
#define K_GAME__COMPONENT_MANAGER_MAP_H

#include <stddef.h>

struct k_component_manager;
struct k_room;

int k__component_manager_map_init(void);

void k__component_manager_map_free(void);

int k__component_manager_map_add(struct k_room *room, struct k_component_manager *manager);

void k__component_manager_map_del(struct k_room *room, struct k_component_manager_type *manager_type);

struct k_component_manager *k__component_manager_map_find(struct k_room *room, struct k_component_manager_type *manager_type);

void k__room_del_all_component_managers(struct k_room *room);

#endif
