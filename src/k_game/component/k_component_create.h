#ifndef K_GAME__COMPONENT_CREATE_H
#define K_GAME__COMPONENT_CREATE_H

#include "k_list.h"

struct k_object;
struct k_component_type;

struct k_component {

    struct k_list_node *iter_node;

    struct k_object *object;

    void *data;
};

struct k_component *k__create_component(struct k_component_type *component_type, struct k_object *object, void *params);

struct k_component *k__destroy_component(struct k_component *component);

#endif
