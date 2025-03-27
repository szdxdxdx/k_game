#ifndef YX_STATE_MACHINE_H
#define YX_STATE_MACHINE_H

#include "k_game.h"

struct yx_state_machine;

int yx_define_component_state_machine(void);

struct yx_state_machine *yx_object_add_state_machine(struct k_object *object);

void yx_object_del_state_machine(struct yx_state_machine *machine);

struct yx_state_machine_state {
    void (*fn_enter)(struct k_object *object);
    void (*fn_step)(struct k_object *object);
    void (*fn_exit)(struct k_object *object);
};

void yx_state_machine_change_state(struct yx_state_machine *machine, struct yx_state_machine_state *state);

#endif
