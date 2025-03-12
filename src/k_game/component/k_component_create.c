#include "k_game_alloc.h"

#include "../object/k_object.h"

#include "./k_component.h"

struct k_component *k__component_create(const struct k_component_type *component_type, struct k_object *object, void *params) {

    struct k_component *component = k_malloc(sizeof(struct k_component));
    if (NULL == component)
        goto malloc_failed;

    void *data = NULL;
    if (0 != component_type->data_size) {
        if (NULL == (data = k_malloc(component_type->data_size)))
            goto malloc_data_failed;
    }

    component->type = component_type;
    component->data = data;
    k_list_init(&component->callback_list);

    component->object = object;
    k_list_add_tail(&object->component_list, &component->list_node);

    if (0 != component_type->fn_init(component, params))
        goto fn_create_failed;

    return component;

fn_create_failed:
    k_list_del(&component->list_node);
    k_component_del_all_callbacks(component);
    k_free(component->data);
malloc_data_failed:
    k_free(component);
malloc_failed:
    return NULL;
}

void k__component_destroy(struct k_component *component) {

    if (component->type->fn_fini != NULL)
        component->type->fn_fini(component);

    k_list_del(&component->list_node);
    k_component_del_all_callbacks(component);

    k_free(component->data);
    k_free(component);
}

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
