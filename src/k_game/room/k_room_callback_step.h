#ifndef K_GAME__ROOM_CALLBACK_STEP_H
#define K_GAME__ROOM_CALLBACK_STEP_H

#include "k_list.h"

#include "./k_room_callback.h"

struct k_room;

struct k_room_step_callback {

    struct k_list_node pending_list_node;

    struct k_list_node callbacks_list_node;

    struct k_list *recycle_list;

    struct k_list_node recycle_list_node;

    struct k_room_callback impl;

    void (*fn_callback)(void *data);

    void *data;
};

struct k_room_step_callbacks_list {

    struct k_list callbacks_list;

    struct k_list pending_list;

    struct k_list recycle_list;
};

void k__room_init_step_callbacks_list(struct k_room_step_callbacks_list *list);

void k__room_cleanup_step_callbacks_list(struct k_room_step_callbacks_list *list);

void k__room_exec_step_callbacks(struct k_room_step_callbacks_list *list);

void k__room_flush_pending_step_callbacks(struct k_room_step_callbacks_list *list);

void k__room_cleanup_recycled_step_callbacks(struct k_room_step_callbacks_list *list);

#endif
