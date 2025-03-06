#ifndef K_GAME__OBJECT_CALLBACK_H
#define K_GAME__OBJECT_CALLBACK_H

#include "k_list.h"

struct k_room_callback;
struct k_object;

struct k_object_callback {

    struct k_list_node object_callback_list_node;

    struct k_room_callback *room_callback;

    struct k_object *object;

    union {
        void (*fn_step_begin_callback)(struct k_object *object);
        void (*fn_alarm_callback)     (struct k_object *object, int timeout_diff);
        void (*fn_step_callback)      (struct k_object *object);
        void (*fn_draw_callback)      (struct k_object *object);
        void (*fn_step_end_callback)  (struct k_object *object);
    };
};

void k__object_init_callback_list(struct k_object *object);

void k__object_cleanup_callback_list(struct k_object *object);

#endif
