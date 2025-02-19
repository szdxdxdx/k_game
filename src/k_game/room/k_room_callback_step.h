#ifndef K_GAME__ROOM_CALLBACK_STEP_H
#define K_GAME__ROOM_CALLBACK_STEP_H

#include "k_list.h"

struct k_room_step_callbacks_storage {

    struct k_list pending_list;

    struct k_list callbacks_list;

    struct k_list recycle_list;
};

#endif
