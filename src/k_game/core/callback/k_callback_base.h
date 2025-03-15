#ifndef K_GAME__CALLBACK_BASE_H
#define K_GAME__CALLBACK_BASE_H

#include <stdint.h>

enum k_callback_type {

    K_ROOM_CALLBACK       = 0,
    K_OBJECT_CALLBACK     ,
    K_COMPONENT_CALLBACK  ,

    K_STEP_BEGIN_CALLBACK = 0,
    K_ALARM_CALLBACK      ,
    K_STEP_CALLBACK       ,
    K_DRAW_CALLBACK       ,
    K_STEP_END_CALLBACK   ,
};

struct k_callback {

    uint8_t is_deleted;

    uint8_t event;

    uint8_t context;
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
