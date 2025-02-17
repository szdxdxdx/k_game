#ifndef K_GAME__ROOM_CALLBACK_H
#define K_GAME__ROOM_CALLBACK_H

#include "k_list.h"

struct k_room;

struct k_room_callback {

    void (*fn_del_self)(struct k_room_callback *self);
};

struct k_room_callback_list {

    struct k_list list;
};

void k__room_callback_list_init(struct k_room_callback_list *list);

void k__room_callback_list_clean(struct k_room_callback_list *list);

struct k_room_callback *k__room_callback_list_add(struct k_room_callback_list *list, void (*fn_callback)(void *data), void *data);

void k__room_callback_list_exec_all(struct k_room_callback_list *list);

#endif
