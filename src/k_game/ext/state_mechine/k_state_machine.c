#include "./_internal.h"

void k_state_machine_change_state(struct k_state_machine *machine, struct k_state_machine_state *state) {

    if (NULL != machine->fn_exit) {
        machine->fn_exit(machine->object);
        machine->fn_exit = NULL;
    }

    if (NULL == state) {
        machine->fn_step = NULL;
        machine->fn_exit = NULL;
        return;
    }

    if (state->fn_enter != NULL)
        state->fn_enter(machine->object);

    machine->fn_step = state->fn_step;
    machine->fn_exit = state->fn_exit;
}

static void yx_state_machine_step(struct k_component *component) {
    struct k_state_machine *machine = k_component_get_data(component);

    if (machine->fn_step != NULL) {
        machine->fn_step(machine->object);
    }
}

int state_machine_init(struct k_component *component, void *params) {
    (void *)params;

    struct k_component_callback *callback = k_component_add_step_callback(component, yx_state_machine_step);
    if (NULL == callback)
        return -1;

    struct k_state_machine *machine = k_component_get_data(component);
    machine->component = component;
    machine->object    = k_component_get_object(component);
    machine->fn_step   = NULL;
    machine->fn_exit   = NULL;
    return 0;
}

void state_machine_fini(struct k_component *component) {
    struct k_state_machine *machine = k_component_get_data(component);

    if (NULL != machine->fn_exit) {
        machine->fn_exit(machine->object);
    }
}

static struct k_component_type *yx_component_type_state_machine;

int k__define_component_state_machine(void) {

    struct k_component_entity_config config = K_COMPONENT_ENTITY_CONFIG_INIT;
    config.data_size = sizeof(struct k_state_machine);
    config.fn_init   = state_machine_init;
    config.fn_fini   = state_machine_fini;

    struct k_component_type *type = k_component_define(NULL, &config);
    if (NULL == type)
        return -1;

    yx_component_type_state_machine = type;
    return 0;
}

struct k_state_machine *k_object_add_state_machine(struct k_object *object) {

    struct k_component *component = k_object_add_component(object, yx_component_type_state_machine, NULL);
    if (NULL == component)
        return NULL;

    return k_component_get_data(component);
}

void k_object_del_state_machine(struct k_state_machine *machine) {

    if (NULL != machine)
        k_object_del_component(machine->component);
}
