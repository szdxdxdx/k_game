#include "./k_component.h"

#include "../object/k_object.h"
#include "../room/k_room.h"

/* region [component_create] */

struct k_component *k__component_create(struct k_component_type *component_type, struct k_object *object, void *params) {
    struct k_component_entity_type *entity_type = &component_type->entity_type;
    struct k_component_manager_type *manager_type = component_type->manager_type;

    struct k_component *component = k_malloc(sizeof(struct k_component));
    if (NULL == component)
        goto malloc_component_failed;

    if (0 == entity_type->data_size) {
        component->data = NULL;
    } else {
        component->data = k_malloc(entity_type->data_size);
        if (NULL == component->data)
            goto malloc_data_failed;
    }

    component->type = component_type;
    if (NULL != manager_type) {
        component->manager = k__component_manager_map_find(object->room, manager_type);
    } else {
        component->manager = NULL;
    }

    k_list_init(&component->callback_list);

    component->object = object;
    k_list_add_tail(&object->component_list, &component->list_node);

    if (NULL != entity_type->fn_init) {
        /* TODO 应该允许在 `fn_init()` 回调中删除自身吗？ */
        if (0 != entity_type->fn_init(component, params))
            goto fn_create_failed;
    }

    return component;

fn_create_failed:
    k_list_del(&component->list_node);
    k__component_free_all_callbacks(component);
    k_free(component->data);
malloc_data_failed:
    k_free(component);
malloc_component_failed:
    return NULL;
}

void k__component_destroy(struct k_component *component) {
    struct k_component_entity_type *entity_type = &component->type->entity_type;

    if (entity_type->fn_fini != NULL) {
        /* TODO 禁止在 `fn_fini()` 回调中再次删除自身 */
        entity_type->fn_fini(component);
    }

    k_list_del(&component->list_node);
    k__component_free_all_callbacks(component);

    k_free(component->data);
    k_free(component);
}

/* endregion */

/* region [component_get] */

void *k_component_get_data(struct k_component *component) {
    return component->data;
}

struct k_object *k_component_get_object(struct k_component *component) {
    return component->object;
}

struct k_component_manager *k_component_get_manager(struct k_component *component) {
    return component->manager;
}

/* endregion */

/* region [object_add_component] */

struct k_component *k_object_add_component(struct k_object *object, struct k_component_type *component_type, void *params) {
    /* TODO assert(NULL != object && NULL == component_type) */

    return k__component_create(component_type, object, params);
}

void k_object_del_component(struct k_component *component) {

    if (NULL != component)
        k__component_destroy(component);
}

void k_object_del_all_components(struct k_object *object) {
    /* TODO assert(NULL != object) */

    struct k_component *component;
    struct k_list *component_list = &object->component_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(component_list, iter, next)) {
        component = container_of(iter, struct k_component, list_node);

        k__component_destroy(component);
    }
}

/* endregion */
