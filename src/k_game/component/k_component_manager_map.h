#ifndef K_GAME__COMPONENT_MANAGER_MAP_H
#define K_GAME__COMPONENT_MANAGER_MAP_H

#include <stddef.h>

struct k_component_manager;

int k__component_manager_map_init(void);

void k__component_manager_map_free(void);

int k__component_manager_map_add(size_t room_id, size_t manager_type_id, struct k_component_manager *manager);

struct k_component_manager *k__component_manager_map_find(size_t room_id, size_t manager_type_id);

#endif
