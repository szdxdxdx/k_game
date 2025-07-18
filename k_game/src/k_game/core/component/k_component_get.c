#include <assert.h>
#include "k_game/core/k_component.h"
#include "./k_component.h"
#include "./k_component_type.h"
#include "./k_component_manager.h"
#include "./k_component_manager_map.h"

#include "../object/k_object.h"
#include "../room/k_room.h"

/* region [component_get] */

void *k_component_get_data(struct k_component *component) {
    assert(NULL != component);
    return component->data;
}

struct k_object *k_component_get_object(struct k_component *component) {
    assert(NULL != component);
    return component->object;
}

struct k_component_manager *k_component_get_manager(struct k_component *component) {
    assert(NULL != component);
    return component->manager;
}

void *k_component_get_manager_data(struct k_component *component) {
    assert(NULL != component);

    if (NULL == component->manager)
        return NULL;

    return component->manager->data;
}

void *k_component_get_object_data(struct k_component *component) {
    assert(NULL != component);
    return component->object->data;
}

/* endregion */

/* region [component_manager_get] */

void *k_component_manager_get_data(struct k_component_manager *manager) {
    return manager->data;
}

struct k_component_manager *k_room_get_component_manager(struct k_component_type *component_type) {
    assert(NULL != component_type);

    if (NULL == component_type)
        return NULL;

    return k__component_manager_map_find(k__room_current, component_type->manager_type);
}

void *k_room_get_component_manager_data(struct k_component_type *component_type) {
    assert(NULL != component_type);

    if (NULL == component_type)
        return NULL;

    struct k_component_manager *manager = k__component_manager_map_find(k__room_current, component_type->manager_type);
    if (NULL == manager)
        return NULL;

    return manager->data;
}

/* endregion */
