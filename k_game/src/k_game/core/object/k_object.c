#include "k_game/core/k_mem_alloc.h"
#include "k_game/core/k_object.h"

#include "./k_object.h"

#include "../room/k_room.h"
#include "../component/k_component.h"

/* region [object_add_destroy_callback] */

/* 这是一个实现 object destroy callback 的临时方案 */

struct k_object_destroy_callback {

    struct k_callback base;

    void (*fn_callback)(struct k_object *object);
};

static void k__object_del_destroy_callback(struct k_callback *callback) {
    struct k_object_destroy_callback *destroy_callback = container_of(callback, struct k_object_destroy_callback, base);

    destroy_callback->fn_callback = NULL;
}

struct k_callback *k_object_add_destroy_callback(struct k_object *object, void (*fn_callback)(struct k_object *object)) {

    if (K__OBJECT_STATE_DESTROYING == object->state)
        return NULL;

    struct k_object_destroy_callback *callback = k__mem_alloc(sizeof(struct k_object_destroy_callback));
    if (NULL == callback)
        return NULL;

    callback->base.fn_del = k__object_del_destroy_callback;
    callback->fn_callback = fn_callback;

    k_list_insert_tail(&object->destroy_callback_list, &callback->base.context_callback_list_node);

    return &callback->base;
}

static void k__object_exec_and_del_destroy_callback(struct k_object *object) {

    struct k_list *callback_list = &object->destroy_callback_list;
    struct k_list_node *iter, *next;
    for(k_list_for_each_s(callback_list, iter, next)) {
        struct k_callback *base = container_of(iter, struct k_callback, context_callback_list_node);
        struct k_object_destroy_callback *callback = container_of(base, struct k_object_destroy_callback, base);

        if (NULL != callback->fn_callback) {
            callback->fn_callback(object);
        }

        k__mem_free(callback);
    }
}

/* endregion */

/* region [object_create] */

#ifdef K__OBJECT_DEBUG_INFO
#undef k_object_create

struct k_object *k_object_debug_create(size_t data_size, const char *debug_info) {
    struct k_object *object = k_object_create(data_size);
    if (NULL == object)
        return NULL;

    object->debug_info = debug_info;
    return object;
}

#endif

struct k_object *k_object_create(size_t data_size) {
    struct k_room *room = k__room_current;

    struct k_object *object = k__object_pool_acquire(&room->object_pool);
    if (NULL == object)
        return NULL;

    object->room = room;

    if (0 == data_size) {
        object->data = NULL;
    } else {
        object->data = k__mem_alloc(data_size);
        if (NULL == object->data) {
            k__object_pool_release(object);
            return NULL;
        }
    }

    k_list_init(&object->callback_list);
    k_list_init(&object->component_list);

    k_list_init(&object->destroy_callback_list);
    object->state = K__OBJECT_STATE_ACTIVE;

    return object;
}

void k_object_destroy(struct k_object *object) {

    if (NULL == object)
        return;

    object->state = K__OBJECT_STATE_DESTROYING;
    k__object_exec_and_del_destroy_callback(object);

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

/* endregion */
