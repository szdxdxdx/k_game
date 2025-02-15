#ifndef K_GAME__ROOM_CALLBACK_DRAW_H
#define K_GAME__ROOM_CALLBACK_DRAW_H

#include "k_list.h"

struct k_room;

struct k_room_draw_callbacks_storage {

    struct k_list depth_lists;
};

void k__room_init_draw_callbacks_storage(struct k_room *room);

void k__room_clean_draw_callbacks_storage(struct k_room *room);

void k__room_exec_draw_callbacks(struct k_room *room);

#endif
