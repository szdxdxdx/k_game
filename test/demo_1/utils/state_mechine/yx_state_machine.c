#include "./yx_state_machine.h"

struct yx_state_machine {

    struct k_component *component;

    void (*fn_step)(struct k_object *object);
    void (*fn_exit)(struct k_object *object);
};

void yx_state_machine_change_state(struct yx_state_machine *machine, struct yx_state_machine_state *state) {

    struct k_object *object = k_component_get_object(machine->component);

    if (machine->fn_exit != NULL) {
        machine->fn_exit(object);
        machine->fn_exit = NULL;
    }

    if (NULL == state) {
        machine->fn_step = NULL;
    } else {
        machine->fn_step = state->fn_step;

        if (state->fn_enter != NULL)
            state->fn_enter(object);
    }
}

static void yx_state_machine_step(struct k_component *component) {
    struct yx_state_machine *machine = k_component_get_data(component);

    if (machine->fn_step != NULL) {
        machine->fn_step(k_component_get_object(machine->component));
    }
}

int state_machine_init(struct k_component *component, void *params) {
    (void *)params;

    struct k_component_callback *callback = k_component_add_step_callback(component, yx_state_machine_step);
    if (NULL == callback)
        return -1;

    struct yx_state_machine *machine = k_component_get_data(component);
    machine->fn_step = NULL;
    machine->fn_exit = NULL;
    return 0;
}

static struct k_component_type *yx_component_type_state_machine;

int yx_define_component_state_machine(void) {

    struct k_component_entity_config config = K_COMPONENT_ENTITY_CONFIG_INIT;
    config.data_size = sizeof(struct yx_state_machine);
    config.fn_init   = state_machine_init;

    struct k_component_type *type = k_component_define(NULL, &config);
    if (NULL == type)
        return -1;

    yx_component_type_state_machine = type;
    return 0;
}

struct yx_state_machine *yx_object_add_state_machine(struct k_object *object) {

    struct k_component *component = k_object_add_component(object, yx_component_type_state_machine, NULL);
    if (NULL == component)
        return NULL;

    return k_component_get_data(component);
}

void yx_object_del_state_machine(struct yx_state_machine *machine) {

    if (NULL != machine)
        k_object_del_component(machine->component);
}
