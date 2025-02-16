#ifndef K_GAME__ROOM_CALLBACK_ENTER_H
#define K_GAME__ROOM_CALLBACK_ENTER_H

#include "k_list.h"

struct k_room;

struct k_room_enter_callbacks_storage {

    struct k_list list;
};

void k__room_init_enter_callbacks_storage(struct k_room *room);

void k__room_clean_enter_callbacks_storage(struct k_room *room);

void k__room_exec_enter_callbacks(struct k_room *room);

#endif
