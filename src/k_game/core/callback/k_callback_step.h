#ifndef K_GAME__CALLBACK_STEP_H
#define K_GAME__CALLBACK_STEP_H

#include "k_list.h"

#include "./k_callback_base.h"

struct k_step_callback_manager {

    struct k_list callback_list;

    struct k_list pending_list;
};

void k__callback_init_step_manager(struct k_step_callback_manager *manager);

void k__callback_deinit_step_manager(struct k_step_callback_manager *manager);

void k__callback_flush_step(struct k_step_callback_manager *manager);

void k__callback_exec_step(struct k_step_callback_manager *manager);

struct k_room_callback *k__callback_add_room_step(struct k_step_callback_manager *manager, struct k_room *room, void *data, void (*fn_callback)(void *data));

struct k_object_callback *k__callback_add_object_step(struct k_step_callback_manager *manager, struct k_object *object, void (*fn_callback)(struct k_object *object));

struct k_component_callback *k__callback_add_component_step(struct k_step_callback_manager *manager, struct k_component *component, void (*fn_callback)(struct k_component *component));

#endif
