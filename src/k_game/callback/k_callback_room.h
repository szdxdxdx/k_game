#ifndef K_GAME__CALLBACK_ROOM_H
#define K_GAME__CALLBACK_ROOM_H

#include "k_list.h"

struct k_room;
struct k_callback;

struct k_room_callback {

    struct k_list_node list_node;

    struct k_callback *base;
};

#endif
