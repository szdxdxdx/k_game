#include <assert.h>

#include "k_game/core/k_callback.h"

#include "../room/k_room.h"
#include "../object/k_object.h"
#include "../component/k_component.h"
#include "../component/k_component_manager.h"

void k_callback_del(struct k_callback *callback) {

    if (NULL == callback)
        return;

    callback->fn_del(callback);
}

/* region [del_all_callbacks] */

/* TODO 目前，del_all_callbacks 值得是删除 step、draw 这类在游戏循环中的回调，之后还有其他事件类型回调，要更改名字以示区分 */

void k__room_del_all_callbacks(struct k_room *room) {

    struct k_callback *callback;
    struct k_list *list = &room->callback_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(list, iter, next)) {
        callback = container_of(iter, struct k_callback, context_callback_list_node);

        callback->fn_del(callback);
    }

    assert(k_list_is_empty(list));
}

void k__object_del_all_callbacks(struct k_object *object) {

    struct k_callback *callback;
    struct k_list *list = &object->callback_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(list, iter, next)) {
        callback = container_of(iter, struct k_callback, context_callback_list_node);

        callback->fn_del(callback);
    }

    assert(k_list_is_empty(list));
}

void k__component_del_all_callbacks(struct k_component *component) {

    struct k_callback *callback;
    struct k_list *list = &component->callback_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(list, iter, next)) {
        callback = container_of(iter, struct k_callback, context_callback_list_node);

        callback->fn_del(callback);
    }

    assert(k_list_is_empty(list));
}

void k__component_manager_del_all_callbacks(struct k_component_manager *manager) {

    struct k_callback *callback;
    struct k_list *list = &manager->callback_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(list, iter, next)) {
        callback = container_of(iter, struct k_callback, context_callback_list_node);

        callback->fn_del(callback);
    }

    assert(k_list_is_empty(list));
}

/* endregion */
