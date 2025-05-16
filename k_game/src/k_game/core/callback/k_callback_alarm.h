#ifndef K__CALLBACK_ALARM_H
#define K__CALLBACK_ALARM_H

#include "k_list.h"

#include "./k_callback_base.h"

struct k_alarm_callback_manager {

    struct k_list callback_list;

    struct k_list pending_list;
};

void k__alarm_callback_manager_init(struct k_alarm_callback_manager *manager);

void k__alarm_callback_manager_deinit(struct k_alarm_callback_manager *manager);

void k__alarm_callback_manager_flush(struct k_alarm_callback_manager *manager);

void k__alarm_callback_manager_exec(struct k_alarm_callback_manager *manager);

#endif
