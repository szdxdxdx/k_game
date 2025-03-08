#include "../room/room.h"
#include "../object/object.h"
#include "./component.h"

struct k_component_callback *k_component_add_alarm_callback(struct k_component *component, void (*fn_callback)(struct k_component *component, int timeout_diff), int delay_ms) {

    struct k_alarm_callback_manager *manager = &component->object->room->alarm_callback_manager;
    struct k_component_callback *callback = k__callback_add_component_alarm(manager, fn_callback, component, delay_ms);
    if (NULL == callback)
        return NULL;

    k_list_add_tail(&component->callback_list, &callback->list_node);
    return callback;
}

struct k_component_callback *k_component_add_step_callback(struct k_component *component, void (*fn_callback)(struct k_component *component)) {

    struct k_step_callback_manager *manager = &component->object->room->step_callback_manager;
    struct k_component_callback *callback = k__callback_add_component_step(manager, fn_callback, component);
    if (NULL == callback)
        return NULL;

    k_list_add_tail(&component->callback_list, &callback->list_node);
    return callback;
}

struct k_component_callback *k_component_add_draw_callback(struct k_component *component, void (*fn_callback)(struct k_component *component), int z_index) {

    struct k_draw_callback_manager *manager = &component->object->room->draw_callback_manager;
    struct k_component_callback *callback = k__callback_add_component_draw(manager, fn_callback, component, z_index);
    if (NULL == callback)
        return NULL;

    k_list_add_tail(&component->callback_list, &callback->list_node);
    return callback;
}

void k_component_del_callback(struct k_component_callback *callback) {

    if (NULL != callback) {
        k__callback_set_deleted(callback->base);
        k_list_del(&callback->list_node);
    }
}

void k_component_del_all_callbacks(struct k_component *component) {

    struct k_component_callback *callback;
    struct k_list *list = &component->callback_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        callback = container_of(iter, struct k_component_callback, list_node);

        k__callback_set_deleted(callback->base);
    }

    k_list_init(&component->callback_list);
}
