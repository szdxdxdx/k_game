#ifndef K__STATE_MACHINE_INTERNAL_H
#define K__STATE_MACHINE_INTERNAL_H

#include "k_game.h"

#include "./_shared.h"

struct k_state_machine {

    struct k_component *component;

    struct k_object *object;

    void (*fn_step)(struct k_object *object);

    void (*fn_exit)(struct k_object *object);
};

#endif
