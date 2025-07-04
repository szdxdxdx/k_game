#include <stddef.h>
#include <assert.h>

#include "yx_state_machine.h"

static struct yx_state_machine_state yx__state_machine_state_default = {
    .on_enter  = NULL,
    .on_update = NULL,
    .on_leave  = NULL,
};

void yx_state_machine_init(struct k_object *object, struct yx_state_machine *sm) {
    sm->object       = object;
    sm->curr_state   = &yx__state_machine_state_default;
    sm->next_state   = NULL;
    sm->change_state = 0;
}

void yx_state_machine_tick(struct yx_state_machine *sm) {

    if ( ! sm->change_state) {
        if (NULL != sm->curr_state->on_update) {
            sm->curr_state->on_update(sm->object);
        }
    }
    else {
        if (NULL != sm->curr_state->on_leave) {
            sm->curr_state->on_leave(sm->object);
        }

        if (NULL != sm->next_state->on_enter) {
            sm->next_state->on_enter(sm->object);
        }

        sm->curr_state = sm->next_state;
        sm->next_state = NULL;
        sm->change_state = 0;
    }
}

void yx_state_machine_change_state(struct yx_state_machine *sm, struct yx_state_machine_state *state) {

    sm->change_state = 1;

    if (NULL != state) {
        sm->next_state = state;
    } else {
        sm->next_state = &yx__state_machine_state_default;
    }
}

struct yx_state_machine_state *yx_state_machine_get_current_state(struct yx_state_machine *sm) {
    return sm->curr_state;
}
