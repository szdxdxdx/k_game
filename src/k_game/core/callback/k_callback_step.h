#ifndef K__CALLBACK_STEP_H
#define K__CALLBACK_STEP_H

#include "k_list.h"

#include "./k_callback_base.h"

struct k_step_callback_manager {

    struct k_list callback_list;

    struct k_list pending_list;
};

void k__step_callback_manager_init(struct k_step_callback_manager *manager);

void k__step_callback_manager_deinit(struct k_step_callback_manager *manager);

void k__step_callback_manager_flush(struct k_step_callback_manager *manager);

void k__step_callback_manager_exec(struct k_step_callback_manager *manager);

struct k_callback *k__step_callback_manager_add_room_callback(struct k_step_callback_manager *manager, struct k_room *room, void *data, void (*fn_callback)(void *data));

struct k_callback *k__step_callback_manager_add_object_callback(struct k_step_callback_manager *manager, struct k_object *object, void (*fn_callback)(struct k_object *object));

struct k_callback *k__step_callback_manager_add_component_callback(struct k_step_callback_manager *manager, struct k_component *component, void (*fn_callback)(struct k_component *component));

/* experimental */
struct k_callback *k__step_callback_manager_add_component_manager_callback(struct k_step_callback_manager *manager, struct k_component_manager *component_manager, void (*fn_callback)(struct k_component_manager *component_manager));

void k__step_callback_manager_del_callback(struct k_callback *callback);

#endif
