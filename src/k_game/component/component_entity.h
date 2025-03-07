#ifndef K_GAME__COMPONENT_ENTITY_H
#define K_GAME__COMPONENT_ENTITY_H

#include "k_list.h"

struct k_component_type;
struct k_object;

struct k_component {

    struct k_list_node object_component_list_node;

    const struct k_component_type *type;

    struct k_object *object;

    struct k_list callback_list;

    void *data;
};

#endif
