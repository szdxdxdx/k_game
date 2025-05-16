#ifndef K__CALLBACK_DRAW_H
#define K__CALLBACK_DRAW_H

#include "k_list.h"
#include "k_int_intrusive_map.h"

#include "./k_callback_base.h"

struct k_draw_callback_manager {

    struct k_list group_list;
    struct k_int_intrusive_map group_map;

    struct k_list callback_pending_list;
};

int k__draw_callback_manager_init(struct k_draw_callback_manager *manager);

void k__draw_callback_manager_deinit(struct k_draw_callback_manager *manager);

void k__draw_callback_manager_flush(struct k_draw_callback_manager *manager);

void k__draw_callback_manager_exec(struct k_draw_callback_manager *manager);

#endif
