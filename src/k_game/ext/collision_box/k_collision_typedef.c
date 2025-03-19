#include "./_internal.h"

struct k_component_type *k__component_type_collision_box = NULL;

int k__component_def_collision_box(void) {

    struct k_component_manager_config manager_config = K_COMPONENT_MANAGER_CONFIG_INIT;
    manager_config.data_size = sizeof(struct k_collision_manager);
    manager_config.fn_init   = k__collision_manager_init;
    manager_config.fn_fini   = k__collision_manager_fini;

    struct k_component_entity_config entity_config = K_COMPONENT_ENTITY_CONFIG_INIT;
    entity_config.data_size = sizeof(struct k_collision_box);
    entity_config.fn_init   = k__collision_box_init;

    k__component_type_collision_box = k_component_define(&manager_config, &entity_config);
    if (NULL == k__component_type_collision_box)
        return -1;

    return 0;
}

/* region [room_add_collision_manager] */

int k_room_add_collision_manager(struct k_room *room) {
    return k_room_add_component_manager(room, k__component_type_collision_box, NULL);;
}

/* endregion */

/* region [object_add_collision_box] */

struct k_collision_box *k_object_add_collision_rectangle(struct k_object *object, struct k_collision_rectangle_config *config) {

    if (NULL == object || NULL == config)
        return NULL;

    config->box_type = K_COLLISION_BOX_RECTANGLE;

    struct k_component *component = k_object_add_component(object, k__component_type_collision_box, config);
    if (NULL == component)
        return NULL;

    return k_component_get_data(component);
}

void k_object_del_collision_box(struct k_collision_box *box) {

    if (NULL != box)
        k_object_del_component(box->component);
}

/* endregion */
