#include "./_internal.h"

struct k_component_type *K__COMPONENT_TYPE_COLLISION = NULL;

int k__component_def_collision_box(void) {

    struct k_component_entity_config entity_config = K_COMPONENT_ENTITY_CONFIG_INIT;
    entity_config.data_size = sizeof(struct k_collision_box);
    entity_config.fn_init   = k__collision_box_init;
    entity_config.fn_fini   = k__collision_box_fini;

    struct k_component_manager_config manager_config = K_COMPONENT_MANAGER_CONFIG_INIT;
    manager_config.data_size = sizeof(struct k_collision_manager);
    manager_config.fn_init   = k__collision_manager_init;
    manager_config.fn_fini   = k__collision_manager_fini;

    K__COMPONENT_TYPE_COLLISION = k_component_define(&manager_config, &entity_config);
    if (NULL == K__COMPONENT_TYPE_COLLISION)
        return -1;

    return 0;
}

/* region [room_add_collision_manager] */

int k_room_add_collision_manager(void) {
    return k_room_add_component_manager(K__COMPONENT_TYPE_COLLISION, NULL);;
}

/* endregion */

/* region [object_add_collision_box] */

static struct k_collision_box *k_object_add_collision_box(struct k_object *object, enum k_collision_box_type type, void *config) {

    struct k_collision_box_config config_;
    config_.type   = type;
    config_.config = config;

    struct k_component *component = k_object_add_component(object, K__COMPONENT_TYPE_COLLISION, &config_);
    if (NULL == component)
        return NULL;
    else
        return k_component_get_data(component);
}

struct k_collision_box *k_object_add_collision_rectangle(struct k_object *object, struct k_collision_rectangle_config *config) {

    if (NULL == config)
        return NULL;
    if (NULL == config->position)
        return NULL;

    return k_object_add_collision_box(object, K_COLLISION_RECTANGLE, config);
}

struct k_collision_box *k_object_add_collision_circle(struct k_object *object, struct k_collision_circle_config *config) {

    if (NULL == config)
        return NULL;
    if (NULL == config->position)
        return NULL;
    if (config->r <= 0)
        return NULL;

    return k_object_add_collision_box(object, K_COLLISION_CIRCLE, config);
}

void k_object_del_collision_box(struct k_collision_box *box) {
    if (NULL != box)
        k_object_del_component(box->component);
}

/* endregion */
