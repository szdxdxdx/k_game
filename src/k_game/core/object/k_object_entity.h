#ifndef K___OBJECT_H
#define K___OBJECT_H

#include "k_list.h"

struct k_object {

    struct k_list_node pool_node;

    struct k_room *room;

    struct k_list callback_list;

    struct k_list component_list;

    void *data;

    void (*fn_destroy)(struct k_object *object);
};

#endif
