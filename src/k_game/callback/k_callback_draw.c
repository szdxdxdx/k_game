#include "k_game_alloc.h"

#include "./k_callback.h"

#include "../room/k_room.h"
#include "../object/k_object.h"
#include "../component/k_component.h"

/* region [layer] */

struct k_draw_callback_layer {

    struct k_list_node layer_list_node;

    struct k_int_map_node z_index_map_node;

    struct k_list callback_list;
};

#define layer_get_z_index(layer) \
    ((layer)->z_index_map_node.key)

static inline void add_new_layer_to_pending_list(struct k_draw_callback_manager *manager, struct k_draw_callback_layer *new_layer) {

    int z_index = layer_get_z_index(new_layer);

    struct k_draw_callback_layer *layer;
    struct k_list *list = &manager->layer_pending_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        layer = container_of(iter, struct k_draw_callback_layer, layer_list_node);

        if (z_index < layer_get_z_index(layer))
            break;
    }

    k_list_add(iter->prev, &new_layer->layer_list_node);
}

static inline void flush_layers(struct k_draw_callback_manager *manager) {

    struct k_list *layer_list = &manager->layer_list;
    struct k_list_node *iter = k_list_get_first(layer_list);

    struct k_draw_callback_layer *pending_layer;
    struct k_list *pending_list = &manager->layer_pending_list;
    struct k_list_node *iter_, *next_;
    for (k_list_for_each_s(pending_list, iter_, next_)) {
        pending_layer = container_of(iter_, struct k_draw_callback_layer, layer_list_node);

        int z_index = layer_get_z_index(pending_layer);

        struct k_draw_callback_layer *layer;
        for (; iter != k_list_tail(layer_list); iter = iter->next) {
            layer = container_of(iter, struct k_draw_callback_layer, layer_list_node);

            if (z_index < layer_get_z_index(layer))
                break;
        }

        iter = iter->prev;

        k_list_del(&pending_layer->layer_list_node);
        k_list_add(iter, &pending_layer->layer_list_node);
    }
}

static inline struct k_draw_callback_layer *find_or_create_layer(struct k_draw_callback_manager *manager, int z_index) {

    struct k_int_map_node *node = k_int_map_get(&manager->z_index_map, z_index);
    if (NULL != node) {
        struct k_draw_callback_layer *layer = container_of(node, struct k_draw_callback_layer, z_index_map_node);
        return layer;
    }

    struct k_draw_callback_layer *new_layer = k_malloc(sizeof(struct k_draw_callback_layer));
    if (NULL == new_layer)
        return NULL;

    k_list_init(&new_layer->callback_list);
    k_int_map_add_directly(&manager->z_index_map, z_index, &new_layer->z_index_map_node);

    add_new_layer_to_pending_list(manager, new_layer);
    return new_layer;
}

/* endregion */

/* region [callback_def] */

struct k_draw_callback {

    struct k_list_node list_node;

    struct k_callback base;

    struct k_draw_callback_layer *layer;
};

struct k_room_draw_callback {

    struct k_draw_callback draw_callback; /* inherited */

    struct k_room_callback room_callback;

    void (*fn_callback)(void *data);

    void *data;
};

struct k_object_draw_callback {

    struct k_draw_callback draw_callback; /* inherited */

    struct k_object_callback object_callback;

    void (*fn_callback)(struct k_object *object);

    struct k_object *object;
};

struct k_component_draw_callback {

    struct k_draw_callback draw_callback; /* inherited */

    struct k_component_callback component_callback;

    void (*fn_callback)(struct k_component *component);

    struct k_component *component;
};

/* endregion */

int k__callback_init_draw_manager(struct k_draw_callback_manager *manager) {

    size_t buckets_num = 64;
    struct k_hash_list *buckets = k_malloc(sizeof(struct k_hash_list) * buckets_num);
    if (NULL == buckets)
        return -1;

    k_int_map_init(&manager->z_index_map, buckets, buckets_num);

    k_list_init(&manager->layer_list);
    k_list_init(&manager->layer_pending_list);
    k_list_init(&manager->callback_pending_list);
    return 0;
}

void k__callback_deinit_draw_manager(struct k_draw_callback_manager *manager) {

    k__callback_flush_draw(manager);

    struct k_draw_callback_layer *layer;
    struct k_list *layer_list = &manager->layer_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(layer_list, iter, next)) {
        layer = container_of(iter, struct k_draw_callback_layer, layer_list_node);

        struct k_draw_callback *draw_callback;
        struct k_list *callback_list = &layer->callback_list;
        struct k_list_node *iter_, *next_;
        for (k_list_for_each_s(callback_list, iter_, next_)) {
            draw_callback = container_of(iter_, struct k_draw_callback, list_node);

            k_free(draw_callback);
        }

        k_free(layer);
    }

    k_free(manager->z_index_map.buckets);
}

void k__callback_flush_draw(struct k_draw_callback_manager *manager) {

    if ( ! k_list_is_empty(&manager->callback_pending_list))
        flush_layers(manager);

    struct k_draw_callback *callback;
    struct k_list *callback_pending_list = &manager->callback_pending_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(callback_pending_list, iter, next)) {
        callback = container_of(iter, struct k_draw_callback, list_node);

        k_list_del(&callback->list_node);
        k_list_add_tail(&callback->layer->callback_list, &callback->list_node);
    }
}

void k__callback_exec_draw(struct k_draw_callback_manager *manager) {

    struct k_draw_callback_layer *layer;
    struct k_list *layer_list = &manager->layer_list;
    struct k_list_node *iter;
    for (k_list_for_each(layer_list, iter)) {
        layer = container_of(iter, struct k_draw_callback_layer, layer_list_node);

        struct k_draw_callback *draw_callback;
        struct k_list *callback_list = &layer->callback_list;
        struct k_list_node *iter_, *next_;
        for (k_list_for_each_s(callback_list, iter_, next_)) {
            draw_callback = container_of(iter_, struct k_draw_callback, list_node);

            if (draw_callback->base.is_deleted) {
                k_list_del(&draw_callback->list_node);
                k_free(draw_callback);
                continue;
            }

            switch (draw_callback->base.context) {
                case K_ROOM_CALLBACK: {
                    struct k_room_draw_callback *callback = (struct k_room_draw_callback *)draw_callback;
                    callback->fn_callback(callback->data);
                    break;
                }
                case K_OBJECT_CALLBACK: {
                    struct k_object_draw_callback *callback = (struct k_object_draw_callback *)draw_callback;
                    callback->fn_callback(callback->object);
                    break;
                }
                case K_COMPONENT_CALLBACK: {
                    struct k_component_draw_callback *callback = (struct k_component_draw_callback *)draw_callback;
                    callback->fn_callback(callback->component);
                    break;
                }
            }
        }
    }
}

struct k_room_callback *k__callback_add_room_draw(struct k_draw_callback_manager *manager, struct k_room *room, void *data, void (*fn_callback)(void *data), int z_index) {

    struct k_draw_callback_layer *layer = find_or_create_layer(manager, z_index);
    if (NULL == layer)
        return NULL;

    struct k_room_draw_callback *callback = k_malloc(sizeof(struct k_room_draw_callback));
    if (NULL == callback)
        return NULL;

    callback->draw_callback.base.context = K_ROOM_CALLBACK;
    callback->draw_callback.base.event   = K_DRAW_CALLBACK;
    callback->draw_callback.base.is_deleted = 0;

    callback->draw_callback.layer = layer;
    k_list_add_tail(&manager->callback_pending_list, &callback->draw_callback.list_node);

    callback->room_callback.base = &callback->draw_callback.base;
    k_list_add_tail(&room->callback_list, &callback->room_callback.list_node);

    callback->fn_callback = fn_callback;
    callback->data        = data;

    return &callback->room_callback;
}

struct k_object_callback *k__callback_add_object_draw(struct k_draw_callback_manager *manager, struct k_object *object, void (*fn_callback)(struct k_object *object), int z_index) {

    struct k_draw_callback_layer *layer = find_or_create_layer(manager, z_index);
    if (NULL == layer)
        return NULL;

    struct k_object_draw_callback *callback = k_malloc(sizeof(struct k_object_draw_callback));
    if (NULL == callback)
        return NULL;

    callback->draw_callback.base.context = K_OBJECT_CALLBACK;
    callback->draw_callback.base.event   = K_DRAW_CALLBACK;
    callback->draw_callback.base.is_deleted = 0;

    callback->draw_callback.layer = layer;
    k_list_add_tail(&manager->callback_pending_list, &callback->draw_callback.list_node);

    callback->object_callback.base = &callback->draw_callback.base;
    k_list_add_tail(&object->callback_list, &callback->object_callback.list_node);

    callback->fn_callback = fn_callback;
    callback->object      = object;

    return &callback->object_callback;
}

struct k_component_callback *k__callback_add_component_draw(struct k_draw_callback_manager *manager, struct k_component *component, void (*fn_callback)(struct k_component *component), int z_index) {

    struct k_draw_callback_layer *layer = find_or_create_layer(manager, z_index);
    if (NULL == layer)
        return NULL;

    struct k_component_draw_callback *callback = k_malloc(sizeof(struct k_component_draw_callback));
    if (NULL == callback)
        return NULL;

    callback->draw_callback.base.context = K_COMPONENT_CALLBACK;
    callback->draw_callback.base.event   = K_DRAW_CALLBACK;
    callback->draw_callback.base.is_deleted = 0;

    callback->draw_callback.layer = layer;
    k_list_add_tail(&manager->callback_pending_list, &callback->draw_callback.list_node);

    callback->component_callback.base = &callback->draw_callback.base;
    k_list_add_tail(&component->callback_list, &callback->component_callback.list_node);

    callback->fn_callback = fn_callback;
    callback->component   = component;

    return &callback->component_callback;
}
