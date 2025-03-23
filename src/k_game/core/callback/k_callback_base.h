#ifndef K_GAME__CALLBACK_BASE_H
#define K_GAME__CALLBACK_BASE_H

#include <stdint.h>

enum k_callback_context {

    K_ROOM_CALLBACK,
    K_OBJECT_CALLBACK,
    K_COMPONENT_CALLBACK,
};

enum k_callback_event {

    K_ALARM_CALLBACK,
    K_STEP_CALLBACK,
    K_DRAW_CALLBACK,
};

enum k_callback_state {

    K_CALLBACK_PENDING,
    K_CALLBACK_ACTIVE,
    K_CALLBACK_EXECUTED,
    K_CALLBACK_DEAD,
};

struct k_callback_base {
    uint8_t context;
    uint8_t event;
    uint8_t state;
};

void k__callback_del(struct k_callback_base *callback);

#endif
