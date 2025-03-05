#ifndef K_GAME__ROOM_CALLBACK_STEP_H
#define K_GAME__ROOM_CALLBACK_STEP_H

#include "k_list.h"

#include "k_game/room_callback.h"

struct k_room_step_callback_registry {

    struct k_list step_begin_callback_list;
    struct k_list step_begin_pending_list;

    struct k_list step_callback_list;
    struct k_list step_pending_list;

    struct k_list step_end_callback_list;
    struct k_list step_end_pending_list;
};

struct k_room_step_callback {

    struct k_list_node list_node;

    struct k_room_callback base;

    void (*fn_callback)(void *data);

    void *data;
};

void k__room_init_step_callback_registry(struct k_room *room);

void k__room_cleanup_step_callback_registry(struct k_room *room);

void k__room_exec_step_begin_callbacks(struct k_room *room);

void k__room_exec_step_callbacks(struct k_room *room);

void k__room_exec_step_end_callbacks(struct k_room *room);

void k__room_flush_step_begin_callbacks(struct k_room *room);

void k__room_flush_step_callbacks(struct k_room *room);

void k__room_flush_step_end_callbacks(struct k_room *room);

#endif
