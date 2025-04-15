#include <assert.h>

#include "k_game/core/k_mem_alloc.h"

#include "./k_callback_draw.h"

#include "../room/k_room.h"
#include "../object/k_object.h"
#include "../component/k_component.h"
#include "../component/k_component_manager.h"

/* region [group] */

struct k_draw_callback_group {

    struct k_list_node group_list_node;
    struct k_int_hash_map_node group_map_node;

    struct k_list layer_list;
    struct k_int_hash_map layer_map;

    struct k_draw_callback_manager *manager;
};

#define group_get_id(group) \
    ((group)->group_map_node.key)

static struct k_draw_callback_group *find_or_create_group(struct k_draw_callback_manager *manager, int group_id) {

    struct k_int_hash_map_node *map_node = k_int_hash_map_get(&manager->group_map, group_id);
    if (NULL != map_node) {
        struct k_draw_callback_group *found_group = container_of(map_node, struct k_draw_callback_group, group_map_node);
        return found_group;
    }

    struct k_draw_callback_group *new_group = k_mem_alloc(sizeof(struct k_draw_callback_group));
    if (NULL == new_group)
        return NULL;

    new_group->manager = manager;

    k_list_init(&new_group->layer_list);

    size_t layer_map_buckets_num = 16;
    struct k_hash_list *buckets = k_mem_alloc(sizeof(struct k_hash_list) * layer_map_buckets_num);
    if (NULL == buckets) {
        k_mem_free(new_group);
        return NULL;
    } else {
        k_int_hash_map_init(&new_group->layer_map, buckets, layer_map_buckets_num);
    }

    k_int_hash_map_add_directly(&manager->group_map, group_id, &new_group->group_map_node);

    struct k_draw_callback_group *group_in_list;
    struct k_list *group_list = &manager->group_list;
    struct k_list_node *iter;
    for (k_list_for_each(group_list, iter)) {
        group_in_list = container_of(iter, struct k_draw_callback_group, group_list_node);

        if (group_id < group_get_id(group_in_list))
            break;
    }
    k_list_insert_before(iter, &new_group->group_list_node);

    return new_group;
}

/* endregion */

/* region [layer] */

struct k_draw_callback_layer {

    struct k_list_node    layer_list_node;
    struct k_int_hash_map_node layer_map_node;

    struct k_list callback_list;

    struct k_draw_callback_group *group;
};

#define layer_get_z_index(layer) \
    ((layer)->layer_map_node.key)

static inline struct k_draw_callback_layer *find_or_create_layer(struct k_draw_callback_manager *manager, int group_id, int z_index) {

    struct k_draw_callback_group *group = find_or_create_group(manager, group_id);
    if (NULL == group)
        return NULL;

    struct k_int_hash_map_node *map_node = k_int_hash_map_get(&group->layer_map, z_index);
    if (NULL != map_node) {
        struct k_draw_callback_layer *found_layer = container_of(map_node, struct k_draw_callback_layer, layer_map_node);
        return found_layer;
    }

    struct k_draw_callback_layer *new_layer = k_mem_alloc(sizeof(struct k_draw_callback_layer));
    if (NULL == new_layer)
        return NULL;

    new_layer->group = group;

    k_list_init(&new_layer->callback_list);

    k_int_hash_map_add_directly(&group->layer_map, z_index, &new_layer->layer_map_node);

    struct k_draw_callback_layer *layer_in_list;
    struct k_list *list = &group->layer_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        layer_in_list = container_of(iter, struct k_draw_callback_layer, layer_list_node);

        if (z_index < layer_get_z_index(layer_in_list))
            break;
    }
    k_list_insert_before(iter, &new_layer->layer_list_node);

    return new_layer;
}

/* endregion */

/* region [callback_def] */

struct k_draw_callback {

    struct k_list_node callback_list_node;
    struct k_list_node pending_list_node;

    struct k_draw_callback_layer *layer;

    struct k_callback base;

    union {
        void (*fn_room_callback)(void *data);
        void (*fn_object_callback)(struct k_object *object);
        void (*fn_component_callback)(struct k_component *component);
        void (*fn_component_manager_callback)(void *data);
    };

    union {
        void *data;
        struct k_object *object;
        struct k_component *component;
    };
};


/* region [add_callback] */

struct k_callback *k__draw_callback_manager_add_room_callback(struct k_draw_callback_manager *manager, struct k_room *room, void *data, void (*fn_callback)(void *data), int z_group, int z_layer) {

    struct k_draw_callback_layer *layer = find_or_create_layer(manager, z_group, z_layer);
    if (NULL == layer)
        return NULL;

    struct k_draw_callback *callback = k_mem_alloc(sizeof(struct k_draw_callback));
    if (NULL == callback)
        return NULL;

    callback->base.context = K__ROOM_CALLBACK;
    callback->base.event   = K__DRAW_CALLBACK;
    callback->base.state   = K__CALLBACK_INACTIVE;

    callback->layer = layer;
    k_list_add_tail(&manager->callback_pending_list, &callback->pending_list_node);
    k_list_node_loop(&callback->callback_list_node);

    callback->fn_room_callback = fn_callback;
    callback->data = data;
    k_list_add_tail(&room->callback_list, &callback->base.context_callback_list_node);

    return &callback->base;
}

struct k_callback *k__draw_callback_manager_add_object_callback(struct k_draw_callback_manager *manager, struct k_object *object, void (*fn_callback)(struct k_object *object), int z_group, int z_layer) {

    struct k_draw_callback_layer *layer = find_or_create_layer(manager, z_group, z_layer);
    if (NULL == layer)
        return NULL;

    struct k_draw_callback *callback = k_mem_alloc(sizeof(struct k_draw_callback));
    if (NULL == callback)
        return NULL;

    callback->base.context = K__OBJECT_CALLBACK;
    callback->base.event   = K__DRAW_CALLBACK;
    callback->base.state   = K__CALLBACK_INACTIVE;

    callback->layer = layer;
    k_list_add_tail(&manager->callback_pending_list, &callback->pending_list_node);
    k_list_node_loop(&callback->callback_list_node);

    callback->fn_object_callback = fn_callback;
    callback->object = object;
    k_list_add_tail(&object->callback_list, &callback->base.context_callback_list_node);

    return &callback->base;
}

struct k_callback *k__draw_callback_manager_add_component_callback(struct k_draw_callback_manager *manager, struct k_component *component, void (*fn_callback)(struct k_component *component), int z_group, int z_layer) {

    struct k_draw_callback_layer *layer = find_or_create_layer(manager, z_group, z_layer);
    if (NULL == layer)
        return NULL;

    struct k_draw_callback *callback = k_mem_alloc(sizeof(struct k_draw_callback));
    if (NULL == callback)
        return NULL;

    callback->base.context = K__COMPONENT_CALLBACK;
    callback->base.event   = K__DRAW_CALLBACK;
    callback->base.state   = K__CALLBACK_INACTIVE;

    callback->layer = layer;
    k_list_add_tail(&manager->callback_pending_list, &callback->pending_list_node);
    k_list_node_loop(&callback->callback_list_node);

    callback->fn_component_callback = fn_callback;
    callback->component = component;
    k_list_add_tail(&component->callback_list, &callback->base.context_callback_list_node);

    return &callback->base;
}

struct k_callback *k__draw_callback_manager_add_component_manager_callback(struct k_draw_callback_manager *manager, struct k_component_manager *component_manager, void *data, void (*fn_callback)(void *data), int z_group, int z_layer) {

    struct k_draw_callback_layer *layer = find_or_create_layer(manager, z_group, z_layer);
    if (NULL == layer)
        return NULL;

    struct k_draw_callback *callback = k_mem_alloc(sizeof(struct k_draw_callback));
    if (NULL == callback)
        return NULL;

    callback->base.context = K__COMPONENT_MANAGER_CALLBACK;
    callback->base.event   = K__DRAW_CALLBACK;
    callback->base.state   = K__CALLBACK_INACTIVE;

    callback->layer = layer;
    k_list_add_tail(&manager->callback_pending_list, &callback->pending_list_node);
    k_list_node_loop(&callback->callback_list_node);

    callback->fn_component_manager_callback = fn_callback;
    callback->data = data;
    k_list_add_tail(&component_manager->callback_list, &callback->base.context_callback_list_node);

    return &callback->base;
}

/* endregion */

/* region [del_callback] */

void k__draw_callback_manager_del_callback(struct k_callback *callback) {

    if (K__CALLBACK_DELETED == callback->state)
        return;

    struct k_draw_callback *draw_callback = container_of(callback, struct k_draw_callback, base);

    switch (callback->state) {
        case K__CALLBACK_INACTIVE:
            callback->state = K__CALLBACK_DELETED;
            break;
        case K__CALLBACK_ACTIVE:
            k_list_add_tail(&draw_callback->layer->group->manager->callback_pending_list, &draw_callback->pending_list_node);
            callback->state = K__CALLBACK_DELETED;
            break;
        default:
            assert(0);
    }

    k_list_del(&draw_callback->base.context_callback_list_node);
}

/* endregion */

/* region [callback_manager] */

int k__draw_callback_manager_init(struct k_draw_callback_manager *manager) {

    size_t buckets_num = 16;
    struct k_hash_list *buckets = k_mem_alloc(sizeof(struct k_hash_list) * buckets_num);
    if (NULL == buckets) {
        return -1;
    } else {
        k_int_hash_map_init(&manager->group_map, buckets, buckets_num);
    }

    k_list_init(&manager->group_list);
    k_list_init(&manager->callback_pending_list);

    return 0;
}

void k__draw_callback_manager_deinit(struct k_draw_callback_manager *manager) {

    k__draw_callback_manager_flush(manager);

    struct k_draw_callback_group *group;
    struct k_list *group_list = &manager->group_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(group_list, iter, next)) {
        group = container_of(iter, struct k_draw_callback_group, group_list_node);

        struct k_draw_callback_layer *layer;
        struct k_list *layer_list = &group->layer_list;
        struct k_list_node *iter_, *next_;
        for (k_list_for_each_s(layer_list, iter_, next_)) {
            layer = container_of(iter_, struct k_draw_callback_layer, layer_list_node);

            struct k_draw_callback *draw_callback;
            struct k_list *callback_list = &layer->callback_list;
            struct k_list_node *iter__, *next__;
            for (k_list_for_each_s(callback_list, iter__, next__)) {
                draw_callback = container_of(iter__, struct k_draw_callback, callback_list_node);

                k_mem_free(draw_callback);
            }
            k_mem_free(layer);
        }
        k_mem_free(group->layer_map.buckets);
    }
    k_mem_free(manager->group_map.buckets);
}

void k__draw_callback_manager_flush(struct k_draw_callback_manager *manager) {

    struct k_draw_callback *callback;
    struct k_list *callback_pending_list = &manager->callback_pending_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(callback_pending_list, iter, next)) {
        callback = container_of(iter, struct k_draw_callback, pending_list_node);

        switch (callback->base.state) {
            case K__CALLBACK_INACTIVE:
                k_list_del(&callback->pending_list_node);
                k_list_node_loop(&callback->pending_list_node);
                k_list_add_tail(&callback->layer->callback_list, &callback->callback_list_node);
                callback->base.state = K__CALLBACK_ACTIVE;
                break;
            case K__CALLBACK_DELETED:
                k_list_del(&callback->callback_list_node);
                k_list_del(&callback->pending_list_node);
                k_mem_free(callback);
                break;
            default:
                assert(0);
        }
    }
}

void k__draw_callback_manager_exec(struct k_draw_callback_manager *manager) {

    struct k_draw_callback_group *group;
    struct k_list *group_list = &manager->group_list;
    struct k_list_node *iter;
    for (k_list_for_each(group_list, iter)) {
        group = container_of(iter, struct k_draw_callback_group, group_list_node);

        struct k_draw_callback_layer *layer;
        struct k_list *layer_list = &group->layer_list;
        struct k_list_node *iter_;
        for (k_list_for_each(layer_list, iter_)) {
            layer = container_of(iter_, struct k_draw_callback_layer, layer_list_node);

            struct k_draw_callback *callback;
            struct k_list *callback_list = &layer->callback_list;
            struct k_list_node *iter__;
            for (k_list_for_each(callback_list, iter__)) {
                callback = container_of(iter__, struct k_draw_callback, callback_list_node);

                if (K__CALLBACK_ACTIVE != callback->base.state)
                    continue;

                switch (callback->base.context) {
                    case K__ROOM_CALLBACK:
                        callback->fn_room_callback(callback->data);
                        break;
                    case K__OBJECT_CALLBACK:
                        callback->fn_object_callback(callback->object);
                        break;
                    case K__COMPONENT_CALLBACK:
                        callback->fn_component_callback(callback->component);
                        break;
                    case K__COMPONENT_MANAGER_CALLBACK:
                        callback->fn_component_manager_callback(callback->data);
                        break;
                    default:
                        assert(0);
                }
            }
        }
    }
}

/* endregion */
