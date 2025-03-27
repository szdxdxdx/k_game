#ifndef K_STATE_MACHINE_INTERNAL_H
#define K_STATE_MACHINE_INTERNAL_H

#include "k_game.h"

#include "./_public.h"

struct k_state_machine {

    struct k_component *component;

    struct k_object *object;

    void (*fn_step)(struct k_object *object);

    void (*fn_exit)(struct k_object *object);
};

#endif
