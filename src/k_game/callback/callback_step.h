#ifndef K_GAME__CALLBACK_STEP_H
#define K_GAME__CALLBACK_STEP_H

#include "k_list.h"

#include "./callback_base.h"

#include "../room/room_callback.h"
#include "../object/object_callback.h"
#include "../component/component_callback.h"

struct k_step_callback_manager {

    struct k_list callback_list;

    struct k_list pending_list;
};

void k__callback_init_step_manager(struct k_step_callback_manager *manager);

void k__callback_deinit_step_manager(struct k_step_callback_manager *manager);

void k__callback_flush_step(struct k_step_callback_manager *manager);

void k__callback_exec_step(struct k_step_callback_manager *manager);

struct k_room_callback *k__callback_add_room_step(struct k_step_callback_manager *manager, void (*fn_callback)(void *), void *data);

struct k_object_callback *k__callback_add_object_step(struct k_step_callback_manager *manager, void (*fn_callback)(struct k_object *object), struct k_object *object);

struct k_component_callback *k__callback_add_component_step(struct k_step_callback_manager *manager, void (*fn_callback)(struct k_component *component), struct k_component *component);

#endif
