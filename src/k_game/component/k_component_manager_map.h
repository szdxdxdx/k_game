#ifndef K_GAME__COMPONENT_MANAGER_MAP_H
#define K_GAME__COMPONENT_MANAGER_MAP_H

#include <stddef.h>

struct k_component_manager;

struct k_component_manager *k__component_manager_map_find(size_t manager_type_id, size_t room_id);

#endif
