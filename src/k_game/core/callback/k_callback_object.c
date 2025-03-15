#include "../room/k_room.h"
#include "../object/k_object.h"

struct k_object_callback *k_object_add_step_begin_callback(struct k_object *object, void (*fn_callback)(struct k_object *object)) {
    return k__callback_add_object_step(&object->room->step_begin_callback_manager, object, fn_callback);
}

struct k_object_callback *k_object_add_alarm_callback(struct k_object *object, void (*fn_callback)(struct k_object *object, int timeout_diff), int delay_ms) {
    return k__callback_add_object_alarm(&object->room->alarm_callback_manager, object, fn_callback, delay_ms);
}

struct k_object_callback *k_object_add_step_callback(struct k_object *object, void (*fn_callback)(struct k_object *object)) {
    return k__callback_add_object_step(&object->room->step_callback_manager, object, fn_callback);
}

struct k_object_callback *k_object_add_draw_callback(struct k_object *object, void (*fn_callback)(struct k_object *object), int z_index) {
    return k__callback_add_object_draw(&object->room->draw_callback_manager, object, fn_callback, z_index);
}

struct k_object_callback *k_object_add_step_end_callback(struct k_object *object, void (*fn_callback)(struct k_object *object)) {
    return k__callback_add_object_step(&object->room->step_end_callback_manager, object, fn_callback);
}

void k_object_del_callback(struct k_object_callback *callback) {

    if (NULL == callback)
        return;

    k_list_del(&callback->list_node);
    k__callback_set_deleted(callback->base);

    {
        /* TODO 禁止重复删除回调？
         *
         * 【room、object、component 的 callback 同理】
         *
         * 理应是不允许用户重复执行删除操作的。但是有可能 k_game 执行了删除，之后用户再删除，
         * 比如 alarm callback，k_game 会执行一次删除，但是仍允许用户在回调中删除自身。
         *
         * 这里将结点自环，保证重复删除回调时不破坏链表结构。
         * 而 `k__callback_set_deleted()` 只是做标记，是可以重读执行的操作。
         */
        k_list_node_loop(&callback->list_node);
    }
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
