#include <assert.h>

#include "k_game/core/k_mem_alloc.h"

#include "k_game/core/k_component.h"
#include "./k_component.h"
#include "./k_component_manager.h"
#include "./k_component_manager_map.h"
#include "./k_component_type.h"

#include "../object/k_object.h"
#include "../callback/k_callback_base.h"

/* region [component_create] */

static struct k_component *k__component_create(struct k_component_type *component_type, struct k_object *object, void *param) {
    struct k_component_entity_type *entity_type   = &component_type->entity_type;
    struct k_component_manager_type *manager_type =  component_type->manager_type;

    struct k_component *component = k__mem_alloc(sizeof(struct k_component));
    if (NULL == component)
        goto malloc_component_failed;

    if (0 == entity_type->data_size) {
        component->data = NULL;
    } else {
        component->data = k__mem_alloc(entity_type->data_size);
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
    k_list_insert_tail(&object->component_list, &component->list_node);

    assert(NULL != entity_type->on_create);
    if (0 != entity_type->on_create(component, param)) /* TODO 不允许在 `fn_init()` 回调中删除自身吗？ */
        goto fn_create_failed;

    return component;

fn_create_failed:
    k_list_remove(&component->list_node);
    k__component_del_all_callbacks(component);
    k__mem_free(component->data);

malloc_data_failed:
    k__mem_free(component);

malloc_component_failed:
    return NULL;
}

static void k__component_destroy(struct k_component *component) {

    struct k_component_entity_type *entity_type = &component->type->entity_type;

    if (entity_type->on_destroy != NULL) {
        /* TODO 禁止在 `fn_fini()` 回调中再次删除自身 */
        entity_type->on_destroy(component);
    }

    k_list_remove(&component->list_node);
    k__component_del_all_callbacks(component);

    k__mem_free(component->data);
    k__mem_free(component);
}

/* endregion */

/* region [object_add_component] */

struct k_component *k_object_add_component(struct k_object *object, struct k_component_type *component_type, void *param) {

    if (NULL == object || NULL == component_type)
        return NULL;
    else
        return k__component_create(component_type, object, param);
}

void k_object_del_component(struct k_component *component) {

    if (NULL == component)
        return;

    k__component_destroy(component);
}

void k__object_del_all_components(struct k_object *object) {
    struct k_component *component;
    struct k_list *component_list = &object->component_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(component_list, iter, next)) {
        component = container_of(iter, struct k_component, list_node);

        k__component_destroy(component);
    }
}

/* endregion */
