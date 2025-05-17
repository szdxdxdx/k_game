#include "k_game/core/k_component.h"
#include "k_game/core/k_callback.h"

#include "k_game/ext/k_state_machine.h"
#include "./k_state_machine_register.h"

struct k_state_machine {

    struct k_component *component;

    struct k_object *object;

    void (*on_step)(struct k_object *object);

    void (*on_exit)(struct k_object *object);
};

/* region [machine_state] */

void k_state_machine_change_state(struct k_state_machine *machine, struct k_state_machine_state *state) {

    /* FIXME 应限制  `machine->on_exit()` 和 `state->fn_enter()` 回调的行为
     * - 不能删除状态机组件自身，不能销毁对象，否则退出回调后会访问到无效内存
     */

    if (NULL != machine->on_exit) {
        machine->on_exit(machine->object);
        machine->on_exit = NULL;
    }

    if (NULL != state) {
        if (NULL != state->on_enter) {
            state->on_enter(machine->object);
        }

        machine->on_step = state->on_step;
        machine->on_exit = state->on_exit;
    }
    else {
        machine->on_step = NULL;
        machine->on_exit = NULL;
    }
}

static void k__state_machine_step(struct k_component *component) {
    struct k_state_machine *machine = k_component_get_data(component);

    if (machine->on_step != NULL) {
        machine->on_step(machine->object);
    }
}

/* endregion */

/* region [machine_init] */

static int k__state_machine_on_create(struct k_component *component, void *param) {
    (void *)param;

    struct k_callback *callback = k_component_add_step_callback(component, k__state_machine_step);
    if (NULL == callback)
        return -1;

    struct k_state_machine *machine = k_component_get_data(component);
    machine->component = component;
    machine->object    = k_component_get_object(component);
    machine->on_step   = NULL;
    machine->on_exit   = NULL;
    return 0;
}

static void k__state_machine_on_destroy(struct k_component *component) {
    struct k_state_machine *machine = k_component_get_data(component);

    if (NULL != machine->on_exit) {
        machine->on_exit(machine->object);
    }
}

/* endregion */

/* region [component_type_register] */

static struct k_component_type *k__state_machine_component_type;

int k__component_type_register_state_machine(void) {

    struct k_component_entity_config config = K_COMPONENT_ENTITY_CONFIG_INIT;
    config.data_size  = sizeof(struct k_state_machine);
    config.on_create  = k__state_machine_on_create;
    config.on_destroy = k__state_machine_on_destroy;

    struct k_component_type *type = k_component_type_register(NULL, &config);
    if (NULL == type)
        return -1;

    k__state_machine_component_type = type;
    return 0;
}

/* endregion */

/* region [object_add_component] */

struct k_state_machine *k_object_add_state_machine(struct k_object *object) {

    struct k_component *component = k_object_add_component(object, k__state_machine_component_type, NULL);
    if (NULL == component)
        return NULL;

    return k_component_get_data(component);
}

void k_object_del_state_machine(struct k_state_machine *machine) {

    if (NULL == machine)
        return;

    k_object_del_component(machine->component);
}

/* endregion */
