#ifndef K_GAME__COMPONENT_TYPE_H
#define K_GAME__COMPONENT_TYPE_H

#include "k_list.h"
#include "k_str_map.h"

struct k_component;

struct k_component_registry {

    struct k_list components_list;

    struct k_str_map name_map;
};

struct k_component_registry_node {

    struct k_list_node iter_node;

    struct k_str_map_node name_map_node;
};

void k__component_registry_init(void);

void k__component_registry_deinit(void);

int k__component_registry_add(struct k_component_registry_node *node, const char *component_name);

struct k_component_type {

    struct k_component_registry_node registry_node;

    size_t data_size;

    int (*fn_create)(struct k_component *component, void *params);

    void (*fn_destroy)(struct k_component *component);
};

#endif
