#ifndef K_GAME__CALLBACK_CONTEXT_H
#define K_GAME__CALLBACK_CONTEXT_H

#include "k_list.h"

#include "./k_callback_base.h"

#include "../room/k_room.h"
#include "../object/_public.h"
#include "../component/k_component.h"

struct k_room_callback {

    struct k_list_node list_node;

    struct k_callback_base *base;
};

struct k_object_callback {

    struct k_list_node list_node;

    struct k_callback_base *base;
};

struct k_component_callback {

    struct k_list_node list_node;

    struct k_callback_base *base;
};

void k__room_free_all_callbacks(struct k_room *room);

void k__object_free_all_callbacks(struct k_object *object);

void k__component_free_all_callbacks(struct k_component *component);

#endif
