#include "k_game/alloc.h"
#include "k_game/object.h"
#include "k_game/component.h"
#include "../object/k_object_entity.h"
#include "./k_component_type.h"
#include "./k_component_callback.h"

/* region [create_component] */

static struct k_component *k__create_component(const struct k_component_type *component_type, struct k_object *object, void *params) {

    struct k_component *component = k_malloc(sizeof(struct k_component));
    if (NULL == component)
        goto malloc_failed;

    void *data = NULL;
    if (0 != component_type->data_size) {
        if (NULL == (data = k_malloc(component_type->data_size)))
            goto malloc_data_failed;
    }

    component->data = data;
    component->object = object;
    k__component_init_callbacks_list(component);

    k_list_add_tail(&object->components.list, &component->iter_node);

    if (0 != component_type->fn_create(component, params))
        goto fn_create_failed;

    return component;

fn_create_failed:
    k_list_del(&component->iter_node);
    k_free(component->data);
malloc_data_failed:
    k_free(component);
malloc_failed:
    return NULL;
}

static void k__destroy_component(struct k_component *component) {

    if (NULL != component->type)
        component->type->fn_destroy(component);

    k__component_cleanup_callbacks_list(component);

    k_list_del(&component->iter_node);
    k_free(component->data);
    k_free(component);
}

/* endregion */

/* region [component_entity] */

void *k_component_get_data(struct k_component *component) {
    return component->data;
}

/* endregion */

/* region [object_add_component] */

void k__object_init_components_list(struct k_object *object) {
    struct k_object_component_list *components_list = &object->components;

    k_list_init(&components_list->list);
}

void k__object_cleanup_components_list(struct k_object *object) {
    struct k_object_component_list *components_list = &object->components;

    /* TODO */
}

struct k_component *k_object_add_component(struct k_object *object, struct k_component_type *component_type, void *params) {
    /* TODO assert NULL != object */
    return k__create_component(component_type, object, params);
}

void k_object_del_component(struct k_object *object, struct k_component *component) {
    /* TODO assert NULL != object */
    k__destroy_component(component);
}

/* endregion */
