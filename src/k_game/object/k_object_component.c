#include "../component/k_component.h"

#include "./k_object.h"

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
