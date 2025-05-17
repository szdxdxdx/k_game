#ifndef K__OBJECT_H
#define K__OBJECT_H

#include "k_list.h"

enum k_object_state {
    K__OBJECT_STATE_ACTIVE,
    K__OBJECT_STATE_DESTROYING,
};

struct k_object {

    void *data;

    struct k_list_node pool_node;

    struct k_room *room;

    struct k_list callback_list;

    struct k_list component_list;

    enum k_object_state state;

    /* tmp */
    struct k_list destroy_callback_list;
};

#endif
