#include "./k_callback.h"

struct k_component_callback *k_component_add_step_begin_callback(struct k_component *component, void (*fn_callback)(struct k_component *component)) {
    return k__callback_add_component_step(&component->object->room->step_begin_callback_manager, component, fn_callback);
}

struct k_component_callback *k_component_add_alarm_callback(struct k_component *component, void (*fn_callback)(struct k_component *component, int timeout_diff), int delay_ms) {
    return k__callback_add_component_alarm(&component->object->room->alarm_callback_manager, component, fn_callback, delay_ms);
}

struct k_component_callback *k_component_add_step_callback(struct k_component *component, void (*fn_callback)(struct k_component *component)) {
    return k__callback_add_component_step(&component->object->room->step_callback_manager, component, fn_callback);
}

struct k_component_callback *k_component_add_draw_callback(struct k_component *component, void (*fn_callback)(struct k_component *component), int z_index) {
    return k__callback_add_component_draw(&component->object->room->draw_callback_manager, component, fn_callback, z_index);
}

struct k_component_callback *k_component_add_step_end_callback(struct k_component *component, void (*fn_callback)(struct k_component *component)) {
    return k__callback_add_component_step(&component->object->room->step_end_callback_manager, component, fn_callback);
}

void k_component_del_callback(struct k_component_callback *callback) {

    if (NULL == callback)
        return;

    k__callback_defer_del(callback->base);

    k_list_del(&callback->list_node);
    k_list_node_loop(&callback->list_node);
}

void k_component_del_all_callbacks(struct k_component *component) {

    struct k_component_callback *callback;
    struct k_list *list = &component->callback_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        callback = container_of(iter, struct k_component_callback, list_node);

        k__callback_defer_del(callback->base);
    }

    k_list_init(&component->callback_list);
}
