#ifndef K__COMPONENT_H
#define K__COMPONENT_H

#include <stddef.h>

#include "k_list.h"

struct k_object;
struct k_component_type;
struct k_component_manager;

struct k_component {

    struct k_list_node list_node;

    struct k_component_type *type;

    struct k_object *object;

    struct k_component_manager *manager;

    struct k_list callback_list;

    void *data;
};

void k__object_del_all_components(struct k_object *object);

#endif
