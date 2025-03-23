#include "../game/_public.h"

#include "./_internal.h"

/* region [room_callback] */

struct k_room_callback *k_room_add_step_begin_callback(void *data, void (*fn_callback)(void *data)) {
    struct k_room *room = k__game.current_room;
    return k__callback_add_room_step(&room->step_begin_callback_manager, room, data, fn_callback);
}

struct k_room_callback *k_room_add_alarm_callback(void *data, void (*fn_callback)(void *data, int timeout_diff), int delay_ms) {
    struct k_room *room = k__game.current_room;
    return k__callback_add_room_alarm(&room->alarm_callback_manager, room, data, fn_callback, delay_ms);
}

struct k_room_callback *k_room_add_step_callback(void *data, void (*fn_callback)(void *data)) {
    struct k_room *room = k__game.current_room;
    return k__callback_add_room_step(&room->step_callback_manager, room, data, fn_callback);
}

struct k_room_callback *k_room_add_draw_callback(void *data, void (*fn_callback)(void *data), int z_index) {
    struct k_room *room = k__game.current_room;
    return k__callback_add_room_draw(&room->draw_callback_manager, room, data, fn_callback, 0, z_index);
}

struct k_room_callback *k_room_add_step_end_callback(void *data, void (*fn_callback)(void *data)) {
    struct k_room *room = k__game.current_room;
    return k__callback_add_room_step(&room->step_end_callback_manager, room, data, fn_callback);
}

void k_room_del_callback(struct k_room_callback *callback) {

    if (NULL != callback)
        k__callback_flag_dead(callback->base);
}

void k_room_del_all_callbacks(void) {
    struct k_room *room = k__game.current_room;

    struct k_room_callback *callback;
    struct k_list *list = &room->callback_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        callback = container_of(iter, struct k_room_callback, list_node);

        k__callback_flag_dead(callback->base);
    }

    k_list_init(&room->callback_list);
}

void k__room_free_all_callbacks(struct k_room *room) {

    struct k_room_callback *callback;
    struct k_list *list = &room->callback_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(list, iter, next)) {
        callback = container_of(iter, struct k_room_callback, list_node);

        k_list_node_loop(&callback->list_node);

        k__callback_flag_dead(callback->base);
    }
}

/* endregion */

/* region [object_callback] */

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
    return k__callback_add_object_draw(&object->room->draw_callback_manager, object, fn_callback, 0, z_index);
}

struct k_object_callback *k_object_add_step_end_callback(struct k_object *object, void (*fn_callback)(struct k_object *object)) {
    return k__callback_add_object_step(&object->room->step_end_callback_manager, object, fn_callback);
}

void k_object_del_callback(struct k_object_callback *callback) {

    if (NULL != callback)
        k__callback_flag_dead(callback->base);
}

void k_object_del_all_callbacks(struct k_object *object) {

    struct k_object_callback *callback;
    struct k_list *list = &object->callback_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        callback = container_of(iter, struct k_object_callback, list_node);

        k__callback_flag_dead(callback->base);
    }

    k_list_init(&object->callback_list);
}

void k__object_free_all_callbacks(struct k_object *object) {

    struct k_object_callback *callback;
    struct k_list *list = &object->callback_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(list, iter, next)) {
        callback = container_of(iter, struct k_object_callback, list_node);

        k_list_node_loop(&callback->list_node);

        k__callback_flag_dead(callback->base);
    }
}

/* endregion */

/* region [component_callback] */

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
    return k__callback_add_component_draw(&component->object->room->draw_callback_manager, component, fn_callback, 0, z_index);
}

struct k_component_callback *k_component_add_step_end_callback(struct k_component *component, void (*fn_callback)(struct k_component *component)) {
    return k__callback_add_component_step(&component->object->room->step_end_callback_manager, component, fn_callback);
}

void k_component_del_callback(struct k_component_callback *callback) {

    if (NULL != callback)
        k__callback_flag_dead(callback->base);
}

void k_component_del_all_callbacks(struct k_component *component) {

    struct k_component_callback *callback;
    struct k_list *list = &component->callback_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        callback = container_of(iter, struct k_component_callback, list_node);

        k__callback_flag_dead(callback->base);
    }

    k_list_init(&component->callback_list);
}

void k__component_free_all_callbacks(struct k_component *component) {

    struct k_component_callback *callback;
    struct k_list *list = &component->callback_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(list, iter, next)) {
        callback = container_of(iter, struct k_component_callback, list_node);

        k_list_node_loop(&callback->list_node);

        k__callback_flag_dead(callback->base);
    }
}

/* endregion */
