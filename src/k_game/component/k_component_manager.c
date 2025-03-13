#include "k_game_alloc.h"

#include "./k_component.h"

/* region [manager_create] */

static int k__component_manager_create(struct k_room *room, struct k_component_type *component_type, void *params) {

    struct k_component_manager_type *manager_type = component_type->manager_type;
    if (NULL == manager_type)
        return -1;

#define ptr_offset(p, offset) ((void *)((char *)(p) + (offset)))

    struct k_component_manager *manager = k_malloc(sizeof(struct k_component_manager) + manager_type->data_size);
    if (NULL == manager)
        return -1;

    manager->component_type = component_type;

    if (0 != manager_type->data_size) {
        manager->data = ptr_offset(manager, sizeof(struct k_component_manager));
    } else {
        manager->data = NULL;
    }

    if (0 != k__component_manager_map_add(room, manager))
        goto map_add_failed;

    if (NULL != manager_type->fn_init) {
        if (0 != manager_type->fn_init(manager, params))
            goto fn_init_failed;
    }

    return 0;

fn_init_failed:
    k__component_manager_map_del(room, manager_type);
map_add_failed:
    k_free(manager);
    return -1;
}

/* endregion */

/* region [room_add_manager] */

int k_room_add_component_manager(struct k_room *room, struct k_component_type *component_type, void *params) {
    return k__component_manager_create(room, component_type, params);
}

void k_room_del_component_manager(struct k_room *room, struct k_component_type *component_type) {

    struct k_component_manager *manager = k__component_manager_map_find(room, component_type->manager_type);
    if (NULL == manager)
        return;

    k__component_manager_map_del(room, component_type->manager_type);
}

/* endregion */

/* region [manager_get] */

void *k_component_manager_get_data(struct k_component_manager *manager) {
    return manager->data;
}

/* endregion */
