#ifndef K_GAME__CALLBACK_BASE_H
#define K_GAME__CALLBACK_BASE_H

#include <stdint.h>

enum k_callback_type {

    K_ROOM_CALLBACK       = 0,
    K_OBJECT_CALLBACK     = 1,
    K_COMPONENT_CALLBACK  = 2,

    K_STEP_BEGIN_CALLBACK = 0,
    K_ALARM_CALLBACK      = 1,
    K_STEP_CALLBACK       = 2,
    K_DRAW_CALLBACK       = 3,
    K_STEP_END_CALLBACK   = 4,
};

struct k_callback {

    uint8_t is_deleted;

    uint8_t event;

    uint8_t context;
};

void k__callback_set_deleted(struct k_callback *callback);

#endif
