#include "k_game/core/k_mem_alloc.h"

#include "k_game/core/k_component.h"
#include "./k_component_type.h"
#include "./k_component_manager.h"
#include "./k_component_manager_map.h"

#include "../room/k_room.h"
#include "../callback/k_callback_context.h"

/* region [component_manager_create] */

static int k__component_manager_create(struct k_room *room, struct k_component_type *component_type, void *params) {

    struct k_component_manager_type *manager_type = component_type->manager_type;
    if (NULL == manager_type)
        return -1;

#define ptr_offset(p, offset) ((void *)((char *)(p) + (offset)))

    struct k_component_manager *manager = k_mem_alloc(sizeof(struct k_component_manager) + manager_type->data_size);
    if (NULL == manager)
        return -1;

    manager->component_type = component_type;
    manager->room = room;

    if (0 != manager_type->data_size) {
        manager->data = ptr_offset(manager, sizeof(struct k_component_manager));
    } else {
        manager->data = NULL;
    }

    if (0 != k__component_manager_map_add(room, manager))
        goto map_add_failed;

    k_list_init(&manager->callback_list);

    if (NULL != manager_type->fn_init) {
        if (0 != manager_type->fn_init(manager, params))
            goto fn_init_failed;
    }

    return 0;

fn_init_failed:
    k__component_manager_map_del(room, manager_type);
map_add_failed:
    k_mem_free(manager);
    return -1;
}

void k__component_manager_destroy(struct k_component_manager *manager) {

    struct k_component_manager_type *manager_type = manager->component_type->manager_type;

    if (NULL != manager_type->fn_fini) {
        /* TODO 禁止在 `fn_fini()` 中再次删除自身 */
        manager_type->fn_fini(manager);
    }

    k__component_manager_del_all_callbacks(manager);

    k__component_manager_map_del(manager->room, manager_type);

    k_mem_free(manager);
}

/* endregion */

/* region [room_add_component_manager] */

int k_room_add_component_manager(struct k_component_type *component_type, void *params) {

    if (NULL == component_type)
        return -1;

    return k__component_manager_create(k__current_room, component_type, params);
}

void k__room_del_component_manager(struct k_room *room, struct k_component_type *component_type) {
    /* 【暂未公布的 API】 */

    struct k_component_manager *manager = k__component_manager_map_find(room, component_type->manager_type);
    if (NULL == manager)
        return;

    k__component_manager_destroy(manager);
}

/* endregion */
