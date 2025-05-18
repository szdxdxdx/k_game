#ifndef YX_STATE_MACHINE_H
#define YX_STATE_MACHINE_H

#include "k_game/core/k_game_fwd.h"

struct yx_state_machine_state {
    void (*on_enter)(struct k_object *object);
    void (*on_update)(struct k_object *object);
    void (*on_leave)(struct k_object *object);
};

struct yx_state_machine {
    struct k_object *object;
    struct yx_state_machine_state *curr_state;
    struct yx_state_machine_state *next_state;
    int change_state;
};

void yx_state_machine_init(struct yx_state_machine *sm, struct k_object *object);

void yx_state_machine_tick(struct yx_state_machine *sm);

void yx_state_machine_change_state(struct yx_state_machine *sm, struct yx_state_machine_state *state);

#endif
