#ifndef K_GAME__CALLBACK_ALARM_H
#define K_GAME__CALLBACK_ALARM_H

#include "k_list.h"

#include "./k_callback_base.h"

struct k_alarm_callback_manager {

    struct k_list callback_list;

    struct k_list pending_list;
};

void k__callback_init_alarm_manager(struct k_alarm_callback_manager *manager);

void k__callback_deinit_alarm_manager(struct k_alarm_callback_manager *manager);

void k__callback_flush_alarm(struct k_alarm_callback_manager *manager);

void k__callback_exec_alarm(struct k_alarm_callback_manager *manager);

struct k_room_callback *k__callback_add_room_alarm(struct k_alarm_callback_manager *manager, struct k_room *room, void *data, void (*fn_callback)(void *data, int timeout_diff), int delay_ms);

struct k_object_callback *k__callback_add_object_alarm(struct k_alarm_callback_manager *manager, struct k_object *object, void (*fn_callback)(struct k_object *object, int timeout_diff), int delay_ms);

struct k_component_callback *k__callback_add_component_alarm(struct k_alarm_callback_manager *manager, struct k_component *component, void (*fn_callback)(struct k_component *component, int timeout_diff), int delay_ms);

#endif
