#include "k_game_alloc.h"
#include "k_game/object_entity.h"
#include "k_game_component.h"
#include "k_game/component_type.h"
#include "k_game/component_entity.h"

/* region [object_component_list_add] */

static inline void object_component_list_add(struct k_object *object, struct k_component *component) {
    struct k_list *component_list = &object->components;
    k_list_add_tail(component_list, &component->object_component_list_node);
}

static inline void object_component_list_del(struct k_component *component) {
    k_list_del(&component->object_component_list_node);
}

/* endregion */

/* region [create_component] */

static struct k_component *create_component(const struct k_component_type *component_type, struct k_object *object, void *params) {

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
    object_component_list_add(object, component);

    if (0 != component_type->fn_create(component, params))
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

static inline void destroy_component(struct k_component *component) {

    if (component->type->fn_destroy != NULL)
        component->type->fn_destroy(component);

    object_component_list_del(component);
    k__component_cleanup_callback_list(component);
    k_free(component->data);
    k_free(component);
}

/* endregion */

/* region [object_component_list_init] */

void k__object_init_component_list(struct k_object *object) {
    struct k_list *component_list = &object->components;
    k_list_init(component_list);
}

void k__object_cleanup_component_list(struct k_object *object) {

    struct k_component *component;
    struct k_list *component_list = &object->components;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(component_list, iter, next)) {
        component = container_of(iter, struct k_component, object_component_list_node);

        destroy_component(component);
    }
}

/* endregion */

/* region [object_add_component] */

struct k_component *k_object_add_component(struct k_object *object, struct k_component_type *component_type, void *params) {
    /* TODO assert(NULL != object && NULL == component_type) */
    return create_component(component_type, object, params);
}

void k_object_del_component(struct k_component *component) {

    if (NULL != component)
        destroy_component(component);
}

/* endregion */

/* region [component_entity] */

struct k_object *k_component_get_object(struct k_component *component) {
    return component->object;
}

void *k_component_get_data(struct k_component *component) {
    return component->data;
}

/* endregion */
