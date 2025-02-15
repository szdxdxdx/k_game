#ifndef K_GAME__ROOM_CALLBACK_H
#define K_GAME__ROOM_CALLBACK_H

#include "./k_room_callback_step.h"

struct k_room_callback {

    void (*fn_del_self)(struct k_room_callback *self);
};

#endif
