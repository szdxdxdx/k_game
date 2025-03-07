#ifndef K_GAME__ROOM_CALLBACK_LEAVE_H
#define K_GAME__ROOM_CALLBACK_LEAVE_H

#include "k_list.h"

#include "./room_callback.h"

struct k_room_leave_callback_registry {

    struct k_list callback_list;

    struct k_list pending_list;
};

struct k_room_leave_callback {

    struct k_list_node list_node;

    struct k_room_callback base;

    void (*fn_callback)(void *data);

    void *data;
};

void k__room_init_leave_callback_registry(struct k_room *room);

void k__room_cleanup_leave_callback_registry(struct k_room *room);

void k__room_exec_leave_callbacks(struct k_room *room);

void k__room_flush_leave_callbacks(struct k_room *room);

#endif
