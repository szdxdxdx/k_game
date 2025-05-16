#ifndef K__CALLBACK_BASE_H
#define K__CALLBACK_BASE_H

#include <stdint.h>

#include "k_list.h"

#include "k_game/core/k_game_fwd.h"

struct k_callback {

    struct k_list_node context_callback_list_node;

    void (*fn_del)(struct k_callback *callback);
};

void k__room_del_all_callbacks(struct k_room *room);

void k__object_del_all_callbacks(struct k_object *object);

void k__component_del_all_callbacks(struct k_component *component);

void k__component_manager_del_all_callbacks(struct k_component_manager *manager);

#endif
