#include <assert.h>

#include "k_game/core/k_mem_alloc.h"

#include "./k_callback_draw.h"

#include "../room/k_room.h"
#include "../object/k_object.h"
#include "../component/k_component.h"
#include "../component/k_component_manager.h"

/* region [callback_manager] */

/* region [group] */

struct k_draw_callback_group {

    struct k_list_node group_list_node;
    struct k_int_intrusive_map_node group_map_node;

    struct k_list layer_list;
    struct k_int_intrusive_map layer_map;

    struct k_draw_callback_manager *manager;
};

#define group_get_id(group) \
    ((group)->group_map_node.key)

static struct k_draw_callback_group *find_or_create_group(struct k_draw_callback_manager *manager, int group_id) {

    struct k_int_intrusive_map_node *map_node = k_int_intrusive_map_get(&manager->group_map, group_id);
    if (NULL != map_node) {
        struct k_draw_callback_group *found_group = container_of(map_node, struct k_draw_callback_group, group_map_node);
        return found_group;
    }

    struct k_draw_callback_group *new_group = k__mem_alloc(sizeof(struct k_draw_callback_group));
    if (NULL == new_group)
        return NULL;

    new_group->manager = manager;

    k_list_init(&new_group->layer_list);

    size_t layer_map_buckets_num = 128;
    struct k_hash_list *buckets = k__mem_alloc(sizeof(struct k_hash_list) * layer_map_buckets_num);
    if (NULL == buckets) {
        k__mem_free(new_group);
        return NULL;
    } else {
        k_int_intrusive_map_init(&new_group->layer_map, buckets, layer_map_buckets_num);
    }

    k_int_intrusive_map_add_directly(&manager->group_map, group_id, &new_group->group_map_node);

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

    struct k_list_node layer_list_node;
    struct k_int_intrusive_map_node layer_map_node;

    struct k_list callback_list;

    struct k_draw_callback_group *group;
};

#define layer_get_z_index(layer) \
    ((layer)->layer_map_node.key)

static inline struct k_draw_callback_layer *find_or_create_layer(struct k_draw_callback_manager *manager, int group_id, int z_index) {

    struct k_draw_callback_group *group = find_or_create_group(manager, group_id);
    if (NULL == group)
        return NULL;

    struct k_int_intrusive_map_node *map_node = k_int_intrusive_map_get(&group->layer_map, z_index);
    if (NULL != map_node) {
        struct k_draw_callback_layer *found_layer = container_of(map_node, struct k_draw_callback_layer, layer_map_node);
        return found_layer;
    }

    struct k_draw_callback_layer *new_layer = k__mem_alloc(sizeof(struct k_draw_callback_layer));
    if (NULL == new_layer)
        return NULL;

    new_layer->group = group;

    k_list_init(&new_layer->callback_list);

    k_int_intrusive_map_add_directly(&group->layer_map, z_index, &new_layer->layer_map_node);

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

/* region [def] */

enum k_draw_callback_context {
    K__DRAW_CALLBACK_CONTEXT_ROOM,
    K__DRAW_CALLBACK_CONTEXT_OBJECT,
    K__DRAW_CALLBACK_CONTEXT_COMPONENT,
    K__DRAW_CALLBACK_CONTEXT_COMPONENT_MANAGER,
};

enum k_draw_callback_state {
    K__DRAW_CALLBACK_STATE_INACTIVE,
    K__DRAW_CALLBACK_STATE_ACTIVE,
    K__DRAW_CALLBACK_STATE_DELETED,
};

struct k_draw_callback {

    struct k_list_node callback_list_node;
    struct k_list_node pending_list_node;

    struct k_draw_callback_layer *layer;

    struct k_callback base;

    enum k_draw_callback_state state;

    enum k_draw_callback_context context;

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

/* endregion */

/* region [del] */

static void k__draw_callback_manager_del_callback(struct k_callback *callback) {
    struct k_draw_callback *draw_callback = container_of(callback, struct k_draw_callback, base);

    switch (draw_callback->state) {
        case K__DRAW_CALLBACK_STATE_INACTIVE:
            draw_callback->state = K__DRAW_CALLBACK_STATE_DELETED;
            break;
        case K__DRAW_CALLBACK_STATE_ACTIVE:
            k_list_insert_tail(&draw_callback->layer->group->manager->callback_pending_list, &draw_callback->pending_list_node);
            draw_callback->state = K__DRAW_CALLBACK_STATE_DELETED;
            break;
        case K__DRAW_CALLBACK_STATE_DELETED:
            return;
        default:
            assert(0);
    }

    k_list_remove(&draw_callback->base.context_callback_list_node);
}

/* endregion */

/* region [add] */

static struct k_callback *k__draw_callback_manager_add_room_callback(struct k_draw_callback_manager *manager, struct k_room *room, void *data, void (*fn_callback)(void *data), int z_group, int z_layer) {

    struct k_draw_callback_layer *layer = find_or_create_layer(manager, z_group, z_layer);
    if (NULL == layer)
        return NULL;

    struct k_draw_callback *callback = k__mem_alloc(sizeof(struct k_draw_callback));
    if (NULL == callback)
        return NULL;

    callback->base.fn_del = k__draw_callback_manager_del_callback;

    callback->layer   = layer;
    callback->context = K__DRAW_CALLBACK_CONTEXT_ROOM;
    callback->state   = K__DRAW_CALLBACK_STATE_INACTIVE;

    k_list_insert_tail(&manager->callback_pending_list, &callback->pending_list_node);
    k_list_node_loop(&callback->callback_list_node);

    callback->fn_room_callback = fn_callback;
    callback->data = data;
    k_list_insert_tail(&room->callback_list, &callback->base.context_callback_list_node);

    return &callback->base;
}

static struct k_callback *k__draw_callback_manager_add_object_callback(struct k_draw_callback_manager *manager, struct k_object *object, void (*fn_callback)(struct k_object *object), int z_group, int z_layer) {

    struct k_draw_callback_layer *layer = find_or_create_layer(manager, z_group, z_layer);
    if (NULL == layer)
        return NULL;

    struct k_draw_callback *callback = k__mem_alloc(sizeof(struct k_draw_callback));
    if (NULL == callback)
        return NULL;

    callback->base.fn_del = k__draw_callback_manager_del_callback;

    callback->layer   = layer;
    callback->context = K__DRAW_CALLBACK_CONTEXT_OBJECT;
    callback->state   = K__DRAW_CALLBACK_STATE_INACTIVE;

    callback->layer = layer;
    k_list_insert_tail(&manager->callback_pending_list, &callback->pending_list_node);
    k_list_node_loop(&callback->callback_list_node);

    callback->fn_object_callback = fn_callback;
    callback->object = object;
    k_list_insert_tail(&object->callback_list, &callback->base.context_callback_list_node);

    return &callback->base;
}

static struct k_callback *k__draw_callback_manager_add_component_callback(struct k_draw_callback_manager *manager, struct k_component *component, void (*fn_callback)(struct k_component *component), int z_group, int z_layer) {

    struct k_draw_callback_layer *layer = find_or_create_layer(manager, z_group, z_layer);
    if (NULL == layer)
        return NULL;

    struct k_draw_callback *callback = k__mem_alloc(sizeof(struct k_draw_callback));
    if (NULL == callback)
        return NULL;

    callback->base.fn_del = k__draw_callback_manager_del_callback;

    callback->layer   = layer;
    callback->context = K__DRAW_CALLBACK_CONTEXT_COMPONENT;
    callback->state   = K__DRAW_CALLBACK_STATE_INACTIVE;

    callback->layer = layer;
    k_list_insert_tail(&manager->callback_pending_list, &callback->pending_list_node);
    k_list_node_loop(&callback->callback_list_node);

    callback->fn_component_callback = fn_callback;
    callback->component = component;
    k_list_insert_tail(&component->callback_list, &callback->base.context_callback_list_node);

    return &callback->base;
}

static struct k_callback *k__draw_callback_manager_add_component_manager_callback(struct k_draw_callback_manager *manager, struct k_component_manager *component_manager, void *data, void (*fn_callback)(void *data), int z_group, int z_layer) {

    struct k_draw_callback_layer *layer = find_or_create_layer(manager, z_group, z_layer);
    if (NULL == layer)
        return NULL;

    struct k_draw_callback *callback = k__mem_alloc(sizeof(struct k_draw_callback));
    if (NULL == callback)
        return NULL;

    callback->base.fn_del = k__draw_callback_manager_del_callback;

    callback->layer   = layer;
    callback->context = K__DRAW_CALLBACK_CONTEXT_COMPONENT_MANAGER;
    callback->state   = K__DRAW_CALLBACK_STATE_INACTIVE;

    callback->layer = layer;
    k_list_insert_tail(&manager->callback_pending_list, &callback->pending_list_node);
    k_list_node_loop(&callback->callback_list_node);

    callback->fn_component_manager_callback = fn_callback;
    callback->data = data;
    k_list_insert_tail(&component_manager->callback_list, &callback->base.context_callback_list_node);

    return &callback->base;
}

/* endregion */

/* region [init] */

int k__draw_callback_manager_init(struct k_draw_callback_manager *manager) {

    size_t buckets_num = 64;
    struct k_hash_list *buckets = k__mem_alloc(sizeof(struct k_hash_list) * buckets_num);
    if (NULL == buckets) {
        return -1;
    } else {
        k_int_intrusive_map_init(&manager->group_map, buckets, buckets_num);
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

                k__mem_free(draw_callback);
            }
            k__mem_free(layer);
        }
        k__mem_free(group->layer_map.buckets);
    }
    k__mem_free(manager->group_map.buckets);
}

/* endregion */

/* region [exec] */

void k__draw_callback_manager_flush(struct k_draw_callback_manager *manager) {

    struct k_draw_callback *callback;
    struct k_list *callback_pending_list = &manager->callback_pending_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(callback_pending_list, iter, next)) {
        callback = container_of(iter, struct k_draw_callback, pending_list_node);

        switch (callback->state) {
            case K__DRAW_CALLBACK_STATE_INACTIVE:
                k_list_remove(&callback->pending_list_node);
                k_list_node_loop(&callback->pending_list_node);
                k_list_insert_tail(&callback->layer->callback_list, &callback->callback_list_node);
                callback->state = K__DRAW_CALLBACK_STATE_ACTIVE;
                break;
            case K__DRAW_CALLBACK_STATE_DELETED:
                k_list_remove(&callback->callback_list_node);
                k_list_remove(&callback->pending_list_node);
                k__mem_free(callback);
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

                if (K__DRAW_CALLBACK_STATE_ACTIVE != callback->state)
                    continue;

                switch (callback->context) {
                    case K__DRAW_CALLBACK_CONTEXT_ROOM:
                        callback->fn_room_callback(callback->data);
                        break;
                    case K__DRAW_CALLBACK_CONTEXT_OBJECT:
                        callback->fn_object_callback(callback->object);
                        break;
                    case K__DRAW_CALLBACK_CONTEXT_COMPONENT:
                        callback->fn_component_callback(callback->component);
                        break;
                    case K__DRAW_CALLBACK_CONTEXT_COMPONENT_MANAGER:
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

/* endregion */

/* region [add_callback] */

struct k_callback *k_room_add_draw_callback(void *data, void (*fn_callback)(void *data), int z_group, int z_layer) {
    struct k_room *room = k__room_current;
    return k__draw_callback_manager_add_room_callback(&room->draw_callback_manager, room, data, fn_callback, z_group, z_layer);
}

struct k_callback *k_object_add_draw_callback(struct k_object *object, void (*fn_callback)(struct k_object *object), int z_group, int z_layer) {
    return k__draw_callback_manager_add_object_callback(&object->room->draw_callback_manager, object, fn_callback, z_group, z_layer);
}

struct k_callback *k_component_add_draw_callback(struct k_component *component, void (*fn_callback)(struct k_component *component), int z_group, int z_layer) {
    return k__draw_callback_manager_add_component_callback(&component->object->room->draw_callback_manager, component, fn_callback, z_group, z_layer);
}

struct k_callback *k_component_manager_add_draw_callback(struct k_component_manager *manager, void *data, void (*fn_callback)(void *data), int z_group, int z_layer) {
    return k__draw_callback_manager_add_component_manager_callback(&manager->room->draw_callback_manager, manager, data, fn_callback, z_group, z_layer);
}

/* endregion */
