#include "k_game_alloc.h"
#include "../object/object_component.h"
#include "./component.h"

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
    k__component_init_callback_list(component);

    component->object = object;
    k__object_component_list_add(object, component);

    if (0 != component_type->fn_init(component, params))
        goto fn_create_failed;

    return component;

fn_create_failed:
    k_list_del(&component->object_component_list_node);
    k_free(component->data);
malloc_data_failed:
    k_free(component);
malloc_failed:
    return NULL;
}

void k__component_destroy(struct k_component *component) {

    if (component->type->fn_fini != NULL)
        component->type->fn_fini(component);

    k__object_component_list_del(component);
    k__component_cleanup_callback_list(component);

    k_free(component->data);
    k_free(component);
}
