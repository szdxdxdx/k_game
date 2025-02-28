#ifndef K_GAME__COMPONENT_TYPE_H
#define K_GAME__COMPONENT_TYPE_H

#include "./k_component_registry.h"

struct k_component;

struct k_component_type {

    struct k_component_registry_node registry_node;

    size_t data_size;

    int (*fn_create)(struct k_component *component, void *params);

    void (*fn_destroy)(struct k_component *component);
};

#endif
