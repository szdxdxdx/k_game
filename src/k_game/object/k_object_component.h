#ifndef K_GAME__OBJECT_COMPONENT_H
#define K_GAME__OBJECT_COMPONENT_H

#include "k_list.h"

struct k_object;

struct k_object_component_list {

    struct k_list *list;
};

void k__object_init_components_list(struct k_object *object);

void k__object_cleanup_components_list(struct k_object *object);

#endif
