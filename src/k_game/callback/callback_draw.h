#ifndef K_GAME__CALLBACK_DRAW_H
#define K_GAME__CALLBACK_DRAW_H

#include "k_list.h"

#include "./callback_base.h"

struct k_draw_callback {

    struct k_list_node list_node;

    struct k_callback base;
};

struct k_draw_callback_manager {

    struct k_list **z_index_lists;

    size_t lists_num;

    struct k_list pending_list;
};

#endif
