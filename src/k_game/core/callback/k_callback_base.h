#ifndef K_GAME__CALLBACK_BASE_H
#define K_GAME__CALLBACK_BASE_H

#include <stdint.h>

struct k_room;
struct k_object;
struct k_component;

enum k_callback_context {

    K_ROOM_CALLBACK,
    K_OBJECT_CALLBACK,
    K_COMPONENT_CALLBACK,
};

struct k_room_callback;
struct k_object_callback;
struct k_component_callback;

enum k_callback_event {

    K_ALARM_CALLBACK,
    K_STEP_CALLBACK,
    K_DRAW_CALLBACK,
};

struct k_step_callback;
struct k_alarm_callback;
struct k_draw_callback;

enum k_callback_state {

    K_CALLBACK_PENDING,
    K_CALLBACK_ACTIVE,
    K_CALLBACK_EXECUTED,
    K_CALLBACK_DELETED,
    K_CALLBACK_DEAD,
};

struct k_callback_base {
    uint8_t context;
    uint8_t event;
    uint8_t state;
};

/* FIXME */
void k__callback_flag_deleted(struct k_callback_base *callback);

void k__callback_flag_dead(struct k_callback_base *callback);

#endif
