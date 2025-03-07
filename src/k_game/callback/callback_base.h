#ifndef K_GAME__CALLBACK_BASE_H
#define K_GAME__CALLBACK_BASE_H

#include <stdint.h>

enum k_callback_event_type {

    K_ALARM_CALLBACK,

    K_STEP_CALLBACK,
};

enum k_callback_context_type {

    K_ROOM_CALLBACK,

    K_OBJECT_CALLBACK,
};

struct k_callback {

    uint8_t event;

    uint8_t context;

    uint8_t deleted;
};

void k__callback_set_deleted(struct k_callback *callback);

#endif
