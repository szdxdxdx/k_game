#include <assert.h>

#include "./_internal.h"

/* region [group] */

struct k_draw_callback_group {

    struct k_list_node    group_list_node;
    struct k_int_map_node group_map_node;

    struct k_list    layer_list;
    struct k_int_map layer_map;

    struct k_draw_callback_manager *manager;
};

#define group_get_id(group) \
    ((group)->group_map_node.key)

static struct k_draw_callback_group *find_or_create_group(struct k_draw_callback_manager *manager, int group_id) {

    struct k_int_map_node *map_node = k_int_map_get(&manager->group_map, group_id);
    if (NULL != map_node) {
        struct k_draw_callback_group *found_group = container_of(map_node, struct k_draw_callback_group, group_map_node);
        return found_group;
    }

    struct k_draw_callback_group *new_group = k_malloc(sizeof(struct k_draw_callback_group));
    if (NULL == new_group)
        return NULL;

    new_group->manager = manager;

    k_list_init(&new_group->layer_list);

    size_t layer_map_buckets_num = 16;
    struct k_hash_list *buckets = k_malloc(sizeof(struct k_hash_list) * layer_map_buckets_num);
    if (NULL == buckets) {
        k_free(new_group);
        return NULL;
    } else {
        k_int_map_init(&new_group->layer_map, buckets, layer_map_buckets_num);
    }

    k_int_map_add_directly(&manager->group_map, group_id, &new_group->group_map_node);

    struct k_draw_callback_group *group_in_list;
    struct k_list *group_list = &manager->group_list;
    struct k_list_node *iter;
    for (k_list_for_each(group_list, iter)) {
        group_in_list = container_of(iter, struct k_draw_callback_group, group_list_node);

        if (group_id < group_get_id(group_in_list))
            break;
    }
    k_list_add(iter->prev, &new_group->group_list_node);

    return new_group;
}

/* endregion */

/* region [layer] */

struct k_draw_callback_layer {

    struct k_list_node    layer_list_node;
    struct k_int_map_node layer_map_node;

    struct k_list callback_list;

    struct k_draw_callback_group *group;
};

#define layer_get_z_index(layer) \
    ((layer)->layer_map_node.key)

static inline struct k_draw_callback_layer *find_or_create_layer(struct k_draw_callback_manager *manager, int group_id, int z_index) {

    struct k_draw_callback_group *group = find_or_create_group(manager, group_id);
    if (NULL == group)
        return NULL;

    struct k_int_map_node *map_node = k_int_map_get(&group->layer_map, z_index);
    if (NULL != map_node) {
        struct k_draw_callback_layer *found_layer = container_of(map_node, struct k_draw_callback_layer, layer_map_node);
        return found_layer;
    }

    struct k_draw_callback_layer *new_layer = k_malloc(sizeof(struct k_draw_callback_layer));
    if (NULL == new_layer)
        return NULL;

    new_layer->group = group;

    k_list_init(&new_layer->callback_list);

    k_int_map_add_directly(&group->layer_map, z_index, &new_layer->layer_map_node);

    struct k_draw_callback_layer *layer_in_list;
    struct k_list *list = &group->layer_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        layer_in_list = container_of(iter, struct k_draw_callback_layer, layer_list_node);

        if (z_index < layer_get_z_index(layer_in_list))
            break;
    }
    k_list_add(iter->prev, &new_layer->layer_list_node);

    return new_layer;
}

/* endregion */

/* region [callback_def] */

struct k_draw_callback {

    struct k_list_node callback_list_node;
    struct k_list_node pending_list_node;

    struct k_draw_callback_layer *layer;

    struct k_callback_base base;
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

/* region [add_callback] */

struct k_room_callback *k__draw_callback_manager_add_room_callback(struct k_draw_callback_manager *manager, struct k_room *room, void *data, void (*fn_callback)(void *data), int z_group, int z_layer) {

    struct k_draw_callback_layer *layer = find_or_create_layer(manager, z_group, z_layer);
    if (NULL == layer)
        return NULL;

    struct k_room_draw_callback *callback = k_malloc(sizeof(struct k_room_draw_callback));
    if (NULL == callback)
        return NULL;

    callback->draw_callback.base.context = K_ROOM_CALLBACK;
    callback->draw_callback.base.event   = K_DRAW_CALLBACK;
    callback->draw_callback.base.state   = K_CALLBACK_PENDING;

    callback->draw_callback.layer = layer;
    k_list_add_tail(&manager->callback_pending_list, &callback->draw_callback.pending_list_node);
    k_list_node_loop(&callback->draw_callback.callback_list_node);

    callback->room_callback.base = &callback->draw_callback.base;
    k_list_add_tail(&room->callback_list, &callback->room_callback.list_node);

    callback->fn_callback = fn_callback;
    callback->data        = data;

    return &callback->room_callback;
}

struct k_object_callback *k__draw_callback_manager_add_object_callback(struct k_draw_callback_manager *manager, struct k_object *object, void (*fn_callback)(struct k_object *object), int z_group, int z_layer) {

    struct k_draw_callback_layer *layer = find_or_create_layer(manager, z_group, z_layer);
    if (NULL == layer)
        return NULL;

    struct k_object_draw_callback *callback = k_malloc(sizeof(struct k_object_draw_callback));
    if (NULL == callback)
        return NULL;

    callback->draw_callback.base.context = K_OBJECT_CALLBACK;
    callback->draw_callback.base.event   = K_DRAW_CALLBACK;
    callback->draw_callback.base.state   = K_CALLBACK_PENDING;

    callback->draw_callback.layer = layer;
    k_list_add_tail(&manager->callback_pending_list, &callback->draw_callback.pending_list_node);
    k_list_node_loop(&callback->draw_callback.callback_list_node);

    callback->object_callback.base = &callback->draw_callback.base;
    k_list_add_tail(&object->callback_list, &callback->object_callback.list_node);

    callback->fn_callback = fn_callback;
    callback->object      = object;

    return &callback->object_callback;
}

struct k_component_callback *k__draw_callback_manager_add_component_callback(struct k_draw_callback_manager *manager, struct k_component *component, void (*fn_callback)(struct k_component *component), int z_group, int z_layer) {

    struct k_draw_callback_layer *layer = find_or_create_layer(manager, z_group, z_layer);
    if (NULL == layer)
        return NULL;

    struct k_component_draw_callback *callback = k_malloc(sizeof(struct k_component_draw_callback));
    if (NULL == callback)
        return NULL;

    callback->draw_callback.base.context = K_COMPONENT_CALLBACK;
    callback->draw_callback.base.event   = K_DRAW_CALLBACK;
    callback->draw_callback.base.state   = K_CALLBACK_PENDING;

    callback->draw_callback.layer = layer;
    k_list_add_tail(&manager->callback_pending_list, &callback->draw_callback.pending_list_node);
    k_list_node_loop(&callback->draw_callback.callback_list_node);

    callback->component_callback.base = &callback->draw_callback.base;
    k_list_add_tail(&component->callback_list, &callback->component_callback.list_node);

    callback->fn_callback = fn_callback;
    callback->component   = component;

    return &callback->component_callback;
}

/* endregion */

/* region [del_callback] */

void k__draw_callback_manager_del_callback(struct k_callback_base *callback) {

    if (K_CALLBACK_DELETED == callback->state)
        return;

    struct k_draw_callback *draw_callback = container_of(callback, struct k_draw_callback, base);

    switch (callback->state) {
        case K_CALLBACK_PENDING:
            callback->state = K_CALLBACK_DELETED;
            break;
        case K_CALLBACK_ACTIVE:
            k_list_add_tail(&draw_callback->layer->group->manager->callback_pending_list, &draw_callback->pending_list_node);
            callback->state = K_CALLBACK_DELETED;
            break;
        default:
            assert(0);
    }

    switch (callback->context) {
        case K_ROOM_CALLBACK: {
            struct k_room_draw_callback *room_callback = (struct k_room_draw_callback *)draw_callback;
            k_list_del(&room_callback->room_callback.list_node);
            break;
        }
        case K_OBJECT_CALLBACK: {
            struct k_object_draw_callback *object_callback = (struct k_object_draw_callback *)draw_callback;
            k_list_del(&object_callback->object_callback.list_node);
            break;
        }
        case K_COMPONENT_CALLBACK: {
            struct k_component_draw_callback *component_callback = (struct k_component_draw_callback *)draw_callback;
            k_list_del(&component_callback->component_callback.list_node);
            break;
        }
        default:
            assert(0);
    }
}

/* endregion */

/* region [callback_manager] */

int k__draw_callback_manager_init(struct k_draw_callback_manager *manager) {

    size_t buckets_num = 16;
    struct k_hash_list *buckets = k_malloc(sizeof(struct k_hash_list) * buckets_num);
    if (NULL == buckets) {
        return -1;
    } else {
        k_int_map_init(&manager->group_map, buckets, buckets_num);
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

                k_free(draw_callback);
            }
            k_free(layer);
        }
        k_free(group->layer_map.buckets);
    }
    k_free(manager->group_map.buckets);
}

void k__draw_callback_manager_flush(struct k_draw_callback_manager *manager) {

    struct k_draw_callback *draw_callback;
    struct k_list *callback_pending_list = &manager->callback_pending_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(callback_pending_list, iter, next)) {
        draw_callback = container_of(iter, struct k_draw_callback, pending_list_node);

        switch (draw_callback->base.state) {
            case K_CALLBACK_PENDING:
                k_list_del(&draw_callback->pending_list_node);
                k_list_node_loop(&draw_callback->pending_list_node);
                k_list_add_tail(&draw_callback->layer->callback_list, &draw_callback->callback_list_node);
                draw_callback->base.state = K_CALLBACK_ACTIVE;
                break;
            case K_CALLBACK_DELETED:
                k_list_del(&draw_callback->callback_list_node);
                k_list_del(&draw_callback->pending_list_node);
                k_free(draw_callback);
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

            struct k_draw_callback *draw_callback;
            struct k_list *callback_list = &layer->callback_list;
            struct k_list_node *iter__;
            for (k_list_for_each(callback_list, iter__)) {
                draw_callback = container_of(iter__, struct k_draw_callback, callback_list_node);

                if (K_CALLBACK_ACTIVE != draw_callback->base.state)
                    continue;

                switch (draw_callback->base.context) {
                    case K_ROOM_CALLBACK: {
                        struct k_room_draw_callback *room_callback = (struct k_room_draw_callback *)draw_callback;
                        room_callback->fn_callback(room_callback->data);
                        break;
                    }
                    case K_OBJECT_CALLBACK: {
                        struct k_object_draw_callback *object_callback = (struct k_object_draw_callback *)draw_callback;
                        object_callback->fn_callback(object_callback->object);
                        break;
                    }
                    case K_COMPONENT_CALLBACK: {
                        struct k_component_draw_callback *component_callback = (struct k_component_draw_callback *)draw_callback;
                        component_callback->fn_callback(component_callback->component);
                        break;
                    }
                    default:
                        assert(0);
                }
            }
        }
    }
}

/* endregion */
