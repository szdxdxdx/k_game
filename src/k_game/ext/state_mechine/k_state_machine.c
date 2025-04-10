#include "k_game/core/k_component.h"
#include "k_game/core/k_callback.h"

#include "k_game/ext/k_state_machine.h"
#include "./k_state_machine_typedef.h"

struct k_state_machine {

    struct k_component *component;

    struct k_object *object;

    void (*fn_step)(struct k_object *object);

    void (*fn_exit)(struct k_object *object);
};

/* region [machine_state] */

void k_state_machine_change_state(struct k_state_machine *machine, struct k_state_machine_state state) {

    /* FIXME 应限制  `machine->fn_exit()` 和 `state->fn_enter()` 回调的行为
     * - 不能删除状态机组件自身，不能销毁对象，否则退出回调后会访问到无效内存
     */

    if (NULL != machine->fn_exit) {
        machine->fn_exit(machine->object);
        machine->fn_exit = NULL;
    }

    if (NULL != state.fn_enter)
        state.fn_enter(machine->object);

    machine->fn_step = state.fn_step;
    machine->fn_exit = state.fn_exit;
}

static void k__state_machine_step(struct k_component *component) {
    struct k_state_machine *machine = k_component_get_data(component);

    if (machine->fn_step != NULL)
        machine->fn_step(machine->object);
}

/* endregion */

/* region [machine_init] */

static int k__state_machine_init(struct k_component *component, void *params) {
    (void *)params;

    struct k_callback *callback = k_component_add_step_callback(component, k__state_machine_step);
    if (NULL == callback)
        return -1;

    struct k_state_machine *machine = k_component_get_data(component);
    machine->component = component;
    machine->object    = k_component_get_object(component);
    machine->fn_step   = NULL;
    machine->fn_exit   = NULL;
    return 0;
}

static void k__state_machine_fini(struct k_component *component) {
    struct k_state_machine *machine = k_component_get_data(component);

    if (NULL != machine->fn_exit) {
        machine->fn_exit(machine->object);
    }
}

/* endregion */

/* region [component_define] */

static struct k_component_type *k__state_machine_component_type;

int k__state_machine_component_define(void) {

    struct k_component_entity_config config = K_COMPONENT_ENTITY_CONFIG_INIT;
    config.data_size = sizeof(struct k_state_machine);
    config.fn_init = k__state_machine_init;
    config.fn_fini = k__state_machine_fini;

    struct k_component_type *type = k_component_define(NULL, &config);
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

    if (NULL != machine)
        k_object_del_component(machine->component);
}

/* endregion */
