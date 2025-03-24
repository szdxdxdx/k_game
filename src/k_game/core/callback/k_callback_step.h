#ifndef K_CALLBACK_STEP_H
#define K_CALLBACK_STEP_H

#include "k_list.h"

#include "./k_callback_base.h"

struct k_step_callback_manager {

    struct k_list callback_list;

    struct k_list pending_list;
};

void k__init_step_callback_manager(struct k_step_callback_manager *manager);

void k__deinit_step_callback_manager(struct k_step_callback_manager *manager);

void k__flush_pending_step_callback(struct k_step_callback_manager *manager);

void k__exec_step_callbacks(struct k_step_callback_manager *manager);

struct k_room_callback *k__add_room_step_callback(struct k_step_callback_manager *manager, struct k_room *room, void *data, void (*fn_callback)(void *data));

struct k_object_callback *k__add_object_step_callback(struct k_step_callback_manager *manager, struct k_object *object, void (*fn_callback)(struct k_object *object));

struct k_component_callback *k__add_component_step_callback(struct k_step_callback_manager *manager, struct k_component *component, void (*fn_callback)(struct k_component *component));

void k__del_step_callback(struct k_callback_base *callback);

#endif
