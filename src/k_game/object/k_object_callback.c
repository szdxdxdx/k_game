#include "../room/k_room.h"
#include "./k_object.h"

struct k_object_callback *k_object_add_step_begin_callback(struct k_object *object, void (*fn_callback)(struct k_object *object)) {

    struct k_step_callback_manager *manager = &object->room->step_begin_callback_manager;
    struct k_object_callback *callback = k__callback_add_object_step(manager, fn_callback, object);
    if (NULL == callback)
        return NULL;

    k_list_add_tail(&object->callback_list, &callback->list_node);
    return callback;
}

struct k_object_callback *k_object_add_alarm_callback(struct k_object *object, void (*fn_callback)(struct k_object *object, int timeout_diff), int delay_ms) {

    struct k_alarm_callback_manager *manager = &object->room->alarm_callback_manager;
    struct k_object_callback *callback = k__callback_add_object_alarm(manager, fn_callback, object, delay_ms);
    if (NULL == callback)
        return NULL;

    k_list_add_tail(&object->callback_list, &callback->list_node);
    return callback;
}

struct k_object_callback *k_object_add_step_callback(struct k_object *object, void (*fn_callback)(struct k_object *object)) {

    struct k_step_callback_manager *manager = &object->room->step_callback_manager;
    struct k_object_callback *callback = k__callback_add_object_step(manager, fn_callback, object);
    if (NULL == callback)
        return NULL;

    k_list_add_tail(&object->callback_list, &callback->list_node);
    return callback;
}

struct k_object_callback *k_object_add_draw_callback(struct k_object *object, void (*fn_callback)(struct k_object *object), int z_index) {

    struct k_draw_callback_manager *manager = &object->room->draw_callback_manager;
    struct k_object_callback *callback = k__callback_add_object_draw(manager, fn_callback, object, z_index);
    if (NULL == callback)
        return NULL;

    k_list_add_tail(&object->callback_list, &callback->list_node);
    return callback;
}

struct k_object_callback *k_object_add_step_end_callback(struct k_object *object, void (*fn_callback)(struct k_object *object)) {

    struct k_step_callback_manager *manager = &object->room->step_end_callback_manager;
    struct k_object_callback *callback = k__callback_add_object_step(manager, fn_callback, object);
    if (NULL == callback)
        return NULL;

    k_list_add_tail(&object->callback_list, &callback->list_node);
    return callback;
}

void k_object_del_callback(struct k_object_callback *callback) {

    if (NULL == callback)
        return;

    k_list_del(&callback->list_node);
    k__callback_set_deleted(callback->base);
}

void k_object_del_all_callbacks(struct k_object *object) {

    struct k_object_callback *callback;
    struct k_list *list = &object->callback_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        callback = container_of(iter, struct k_object_callback, list_node);

        k__callback_set_deleted(callback->base);
    }

    k_list_init(&object->callback_list);
}
