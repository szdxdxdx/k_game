#ifndef K_GAME__COMPONENT_CALLBACK_H
#define K_GAME__COMPONENT_CALLBACK_H

#include "k_list.h"

struct k_component;
struct k_room_callback;

struct k_component_callback {

    struct k_list_node component_callback_list_node;

    struct k_room_callback *room_callback;

    struct k_component *component;

    union {
        void (*fn_alarm_callback)(struct k_component *component, int timeout_diff);
        void (*fn_step_callback) (struct k_component *component);
        void (*fn_draw_callback) (struct k_component *component);
    };
};

void k__component_init_callback_list(struct k_component *component);

void k__component_cleanup_callback_list(struct k_component *component);

#endif
