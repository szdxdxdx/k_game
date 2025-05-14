#ifndef K__OBJECT_H
#define K__OBJECT_H

#include "k_list.h"

struct k_object {

    void *data;

    struct k_list_node pool_node;

    struct k_room *room;

    struct k_list callback_list;

    struct k_list component_list;

    void (*on_destroy)(struct k_object *object);
};

#endif
