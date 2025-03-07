#ifndef K_GAME__OBJECT_CALLBACK_H
#define K_GAME__OBJECT_CALLBACK_H

#include "k_list.h"

struct k_object;
struct k_callback;

struct k_object_callback {

    struct k_list_node list_node;

    struct k_callback *base;
};

void k__object_init_callback_list(struct k_object *object);

#endif
