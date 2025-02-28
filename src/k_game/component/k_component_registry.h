#ifndef K_GAME__COMPONENT_REGISTRY_H
#define K_GAME__COMPONENT_REGISTRY_H

#include "k_list.h"
#include "k_str_map.h"

struct k_component_type;

struct k_component_registry {
    struct k_list components_list;
    struct k_str_map name_map;
};

struct k_component_registry_node {
    struct k_str_map_node name_map_node;
    struct k_list_node iter_node;
};

void k__component_registry_init(void);

void k__component_registry_deinit(void);

int k__component_registry_add(struct k_component_type *component_type, const char *type_name);

void k__component_registry_del(struct k_component_type *component_type);

#endif
