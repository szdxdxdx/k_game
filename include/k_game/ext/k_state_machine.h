#ifndef K_STATE_MACHINE_H
#define K_STATE_MACHINE_H

struct k_object;

struct k_state_machine;

struct k_state_machine *k_object_add_state_machine(struct k_object *object);

void k_object_del_state_machine(struct k_state_machine *machine);

struct k_state_machine_state {
    void (*fn_enter)(struct k_object *object);
    void (*fn_step)(struct k_object *object);
    void (*fn_exit)(struct k_object *object);
};

void k_state_machine_change_state(struct k_state_machine *machine, struct k_state_machine_state *state);

#endif
