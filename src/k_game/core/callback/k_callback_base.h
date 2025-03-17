#ifndef K_GAME__CALLBACK_BASE_H
#define K_GAME__CALLBACK_BASE_H

#include <stdint.h>

enum k_callback_state {

    K_CALLBACK_PENDING,
    K_CALLBACK_USER_DEL,
    K_CALLBACK_DELETED,
};

enum k_callback_context {

    K_ROOM_CALLBACK,
    K_OBJECT_CALLBACK,
    K_COMPONENT_CALLBACK,
};

struct k_callback {

    void *_;
};

void k__callback_set_deleted(struct k_callback *callback);

struct k_step_callback;
struct k_alarm_callback;
struct k_draw_callback;

struct k_room;
struct k_room_callback;
struct k_object;
struct k_object_callback;
struct k_component;
struct k_component_callback;

#endif
