#ifndef YX_STATE_MACHINE_H
#define YX_STATE_MACHINE_H

#include "k_game.h"

struct yx_state_machine_node {
    void (*fn_update)
};

struct yx_state_machine {
    int current_state;
    int next_state;
    int (*fn_update)(struct yx_state_machine *state_machine);
    int (*fn_enter)(struct yx_state_machine *state_machine);
    int (*fn_exit)(struct yx_state_machine *state_machine);
};

#endif
