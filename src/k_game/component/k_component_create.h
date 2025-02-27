#ifndef K_GAME__COMPONENT_CREATE_H
#define K_GAME__COMPONENT_CREATE_H

#include "k_list.h"

struct k_object;

struct k_object_component_list {

    struct k_list list;
};

void k__object_init_component_list(struct k_object *object);

void k__object_cleanup_component_list(struct k_object *object);

struct k_component_type;

struct k_component {

    struct k_list_node iter_node;

    struct k_component_type *type;

    struct k_object *object;

    struct k_list callbacks;

    void *data;
};

#endif
