#ifndef K_GAME__CALLBACK_OBJECT_H
#define K_GAME__CALLBACK_OBJECT_H

#include "k_list.h"

struct k_object;
struct k_callback;

struct k_object_callback {

    struct k_list_node list_node;

    struct k_callback *base;
};

#endif
