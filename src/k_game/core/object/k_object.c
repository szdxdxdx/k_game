#include "k_game/core/k_mem_alloc.h"
#include "k_game/core/k_object.h"

#include "./k_object.h"
#include "./k_object_pool.h"

#include "../room/k_room.h"
#include "../component/k_component.h"
#include "../callback/k_callback_context.h"

/* region [object_create] */

struct k_object *k_object_create(size_t data_size) {

    struct k_room *room = k__room_current; /* 获取当前房间 */
    struct k_object *object = k__object_pool_acquire(&room->object_pool); /* 由对象池负责分配对象的内存 */
    if (NULL == object)
        return NULL;
    if (0 == data_size) { /* 分配对象的用户自定义关联数据的内存 */
        object->data = NULL;
    } else {
        object->data = k__mem_alloc(data_size);
        if (NULL == object->data) {
            k__object_pool_release(object);
            return NULL;
        }
    }

    k_list_init(&object->callback_list); /* 初始化对象的事件回调链表和组件链表。销毁对象时，遍历这两个链表，移除事件回调和组件 */
    k_list_init(&object->component_list);

    object->room = room;
    object->on_destroy = NULL;
    return object;
}

void k_object_destroy(struct k_object *object) {

    if (NULL == object)
        return;

    if (NULL != object->on_destroy) {
        object->on_destroy(object);
    }

    k__object_del_all_components(object);
    k__object_del_all_callbacks(object);

    k__mem_free(object->data);
    k__object_pool_release(object);
}

/* endregion */

/* region [object_get/set] */

void *k_object_get_data(struct k_object *object) {
    return object->data;
}

void k_object_set_on_destroy(struct k_object *object, void (*on_destroy)(struct k_object *object)) {
    object->on_destroy = on_destroy;
}

/* endregion */
