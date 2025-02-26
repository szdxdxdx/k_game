#include "../object/k_object_entity.h"
#include "./k_component_create.h"
#include "./k_component_type.h"

/* region [object_components] */

void k__object_init_components_list(struct k_object *object) {
    struct k_object_component_list *components_list = &object->components;

    /* TODO */
}

void k__object_cleanup_components_list(struct k_object *object) {
    struct k_object_component_list *components_list = &object->components;

    /* TODO */
}

/* endregion */

/* region [create_component] */

struct k_component *k__create_component(struct k_component_type *component_type, struct k_object *object, void *params) {

    return NULL;
}

struct k_component *k__destroy_component(struct k_component *component) {

    return NULL;
}

/* endregion */
