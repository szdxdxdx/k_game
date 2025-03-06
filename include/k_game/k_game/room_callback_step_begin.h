#ifndef K_GAME__ROOM_CALLBACK_STEP_BEGIN_H
#define K_GAME__ROOM_CALLBACK_STEP_BEGIN_H

#include "k_list.h"

#include "k_game/room_callback.h"

struct k_room_step_begin_callback_registry {

    struct k_list callback_list;

    struct k_list pending_list;
};

struct k_room_step_begin_callback {

    struct k_list_node list_node;

    struct k_room_callback base;

    void (*fn_callback)(void *data);

    void *data;
};

void k__room_init_step_begin_callback_registry(struct k_room *room);

void k__room_cleanup_step_begin_callback_registry(struct k_room *room);

void k__room_exec_step_begin_callbacks(struct k_room *room);

void k__room_flush_pending_step_begin_callbacks(struct k_room *room);

#endif
