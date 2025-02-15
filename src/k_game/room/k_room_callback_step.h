#ifndef K_GAME__ROOM_CALLBACK_STEP_H
#define K_GAME__ROOM_CALLBACK_STEP_H

#include "k_list.h"

struct k_room;

struct k_room_step_callbacks_storage {

    struct k_list list;
};

void k_room_init_step_callbacks_storage(struct k_room *room);

void k_room_clean_step_callbacks_storage(struct k_room *room);

void k_room_exec_step_callbacks(struct k_room *room);

#endif
