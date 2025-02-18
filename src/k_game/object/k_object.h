#ifndef K_GAME__OBJECT_H
#define K_GAME__OBJECT_H

#include "k_list.h"

struct k_object_pool {

    struct k_list objects_list;
};

struct k_object {

    struct k_list_node object_pool_node;
};

#endif
