#include "k_game_alloc.h"

#include "./callback.h"

/* region [callback_def] */

struct k_room_draw_callback {

    struct k_draw_callback draw_callback;

    struct k_room_callback room_callback;

    void (*fn_callback)(void *data);

    void *data;
};

struct k_object_draw_callback {

    struct k_draw_callback draw_callback;

    struct k_object_callback object_callback;

    void (*fn_callback)(struct k_object *object);

    struct k_object *object;
};

struct k_component_draw_callback {

    struct k_draw_callback draw_callback;

    struct k_component_callback component_callback;

    void (*fn_callback)(struct k_component *component);

    struct k_component *component;
};

/* endregion */

void k__callback_init_draw_manager(struct k_draw_callback_manager *manager) {

    k_list_init(&manager->layer_list);
    k_list_init(&manager->pending_layer_list);
    k_list_init(&manager->pending_callback_list);
}

void k__callback_deinit_draw_manager(struct k_draw_callback_manager *manager) {

    /* TODO */
}

void k__callback_flush_draw(struct k_draw_callback_manager *manager) {

    struct k_draw_callback *callback;
    struct k_list *pending_list = &manager->pending_callback_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(pending_list, iter, next)) {
        callback = container_of(iter, struct k_draw_callback, list_node);

        k_list_del(&callback->list_node);
        k_list_add_tail(&callback->layer->callback_list, &callback->list_node);
    }
}

void k__callback_exec_draw(struct k_draw_callback_manager *manager) {

}

static struct k_draw_callback_layer *find_or_create_layer(struct k_draw_callback_manager *manager, int z_index) {

    /* 先在 z_index_map 中二分查找得到 layer
     * 若找不到，则创建一个新的 layer 添加到 pending_layer_list 中，并修改 z_index_map
     *
     * pending_layer_list 中的 layer 也是有序存储的，使得 layer 归并的时候能更高效
     */
}

struct k_room_callback *k__callback_add_room_draw(struct k_draw_callback_manager *manager, void (*fn_callback)(void *data), void *data, int z_index) {

    struct k_draw_callback_layer *layer = find_or_create_layer(manager, z_index);
    if (NULL == layer)
        return NULL;

    struct k_room_draw_callback *callback = k_malloc(sizeof(struct k_room_draw_callback));
    if (NULL == callback)
        return NULL;

    callback->draw_callback.base.event   = K_DRAW_CALLBACK;
    callback->draw_callback.base.context = K_ROOM_CALLBACK;
    callback->draw_callback.base.deleted = 0;
    callback->draw_callback.layer        = layer;
    callback->room_callback.base         = &callback->draw_callback.base;
    callback->fn_callback                = fn_callback;
    callback->data                       = data;
    k_list_add_tail(&manager->pending_callback_list, &callback->draw_callback.list_node);

    return &callback->room_callback;
}

struct k_object_callback *k__callback_add_object_draw(struct k_draw_callback_manager *manager, void (*fn_callback)(struct k_object *object), struct k_object *object, int z_index) {

    struct k_draw_callback_layer *layer = find_or_create_layer(manager, z_index);
    if (NULL == layer)
        return NULL;

    struct k_object_draw_callback *callback = k_malloc(sizeof(struct k_object_draw_callback));
    if (NULL == callback)
        return NULL;

    callback->draw_callback.base.event   = K_DRAW_CALLBACK;
    callback->draw_callback.base.context = K_OBJECT_CALLBACK;
    callback->draw_callback.base.deleted = 0;
    callback->draw_callback.layer        = layer;
    callback->object_callback.base       = &callback->draw_callback.base;
    callback->fn_callback                = fn_callback;
    callback->object                     = object;
    k_list_add_tail(&manager->pending_callback_list, &callback->draw_callback.list_node);

    return &callback->object_callback;
}

struct k_component_callback *k__callback_add_component_draw(struct k_draw_callback_manager *manager, void (*fn_callback)(struct k_component *component), struct k_component *component, int z_index) {

    struct k_draw_callback_layer *layer = find_or_create_layer(manager, z_index);
    if (NULL == layer)
        return NULL;

    struct k_component_draw_callback *callback = k_malloc(sizeof(struct k_component_draw_callback));
    if (NULL == callback)
        return NULL;

    callback->draw_callback.base.event   = K_DRAW_CALLBACK;
    callback->draw_callback.base.context = K_COMPONENT_CALLBACK;
    callback->draw_callback.base.deleted = 0;
    callback->draw_callback.layer        = layer;
    callback->component_callback.base    = &callback->draw_callback.base;
    callback->fn_callback                = fn_callback;
    callback->component                  = component;
    k_list_add_tail(&manager->pending_callback_list, &callback->draw_callback.list_node);

    return &callback->component_callback;
}
