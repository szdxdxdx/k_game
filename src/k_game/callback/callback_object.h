#ifndef K_GAME__CALLBACK_OBJECT_H
#define K_GAME__CALLBACK_OBJECT_H

#include "./callback_step.h"
#include "./callback_alarm.h"

struct k_object;

struct k_object_callback {

    struct k_list_node object_callback_list_node;

    struct k_callback *base;
};

#endif
