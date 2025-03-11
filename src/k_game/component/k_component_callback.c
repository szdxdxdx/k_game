#include "../room/k_room.h"
#include "../object/k_object.h"
#include "./k_component.h"

struct k_component_callback *k_component_add_step_begin_callback(struct k_component *component, void (*fn_callback)(struct k_component *component)) {

    struct k_step_callback_manager *manager = &component->object->room->step_begin_callback_manager;
    struct k_component_callback *callback = k__callback_add_component_step(manager, component, fn_callback);
    if (NULL == callback)
        return NULL;

    k_list_add_tail(&component->callback_list, &callback->list_node);
    return callback;
}

struct k_component_callback *k_component_add_alarm_callback(struct k_component *component, void (*fn_callback)(struct k_component *component, int timeout_diff), int delay_ms) {

    struct k_alarm_callback_manager *manager = &component->object->room->alarm_callback_manager;
    struct k_component_callback *callback = k__callback_add_component_alarm(manager, component, fn_callback, delay_ms);
    if (NULL == callback)
        return NULL;

    k_list_add_tail(&component->callback_list, &callback->list_node);
    return callback;
}

struct k_component_callback *k_component_add_step_callback(struct k_component *component, void (*fn_callback)(struct k_component *component)) {

    struct k_step_callback_manager *manager = &component->object->room->step_callback_manager;
    struct k_component_callback *callback = k__callback_add_component_step(manager, component, fn_callback);
    if (NULL == callback)
        return NULL;

    k_list_add_tail(&component->callback_list, &callback->list_node);
    return callback;
}

struct k_component_callback *k_component_add_draw_callback(struct k_component *component, void (*fn_callback)(struct k_component *component), int z_index) {

    struct k_draw_callback_manager *manager = &component->object->room->draw_callback_manager;
    struct k_component_callback *callback = k__callback_add_component_draw(manager, component, fn_callback, z_index);
    if (NULL == callback)
        return NULL;

    k_list_add_tail(&component->callback_list, &callback->list_node);
    return callback;
}

struct k_component_callback *k_component_add_step_end_callback(struct k_component *component, void (*fn_callback)(struct k_component *component)) {

    struct k_step_callback_manager *manager = &component->object->room->step_end_callback_manager;
    struct k_component_callback *callback = k__callback_add_component_step(manager, component, fn_callback);
    if (NULL == callback)
        return NULL;

    k_list_add_tail(&component->callback_list, &callback->list_node);
    return callback;
}

void k_component_del_callback(struct k_component_callback *callback) {

    if (NULL == callback)
        return;

    k_list_del(&callback->list_node);
    k__callback_set_deleted(callback->base);
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
