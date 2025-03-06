#include "./object_entity.h"
#include "./object_component.h"
#include "../component/component_entity.h"

void k__object_init_component_list(struct k_object *object) {
    k_list_init(&object->component_list);
}

void k__object_cleanup_component_list(struct k_object *object) {

    struct k_component *component;
    struct k_list *component_list = &object->component_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(component_list, iter, next)) {
        component = container_of(iter, struct k_component, object_component_list_node);

        k__component_destroy(component);
    }
}

void k__object_component_list_add(struct k_object *object, struct k_component *component) {
    k_list_add_tail(&object->component_list, &component->object_component_list_node);
}

void k__object_component_list_del(struct k_component *component) {
    k_list_del(&component->object_component_list_node);
}

struct k_component *k_object_add_component(struct k_object *object, struct k_component_type *component_type, void *params) {
    /* TODO assert(NULL != object && NULL == component_type) */
    return k__component_create(component_type, object, params);
}

void k_object_del_component(struct k_component *component) {

    if (NULL != component)
        k__component_destroy(component);
}

void k_object_del_all_components(struct k_object *object) {
    /* assert( NULL != object) */
    k__object_cleanup_component_list(object);
}
