#ifndef K_GAME__OBJECT_CALLBACK_H
#define K_GAME__OBJECT_CALLBACK_H

#include "k_list.h"

struct k_object;

struct k_object_callbacks_list {

    struct k_list list;
};

void k__object_init_callbacks_list(struct k_object *object);

void k__object_cleanup_callbacks_list(struct k_object *object);

#endif
