#include "yx_comp_camera.h"

int yx_camera_target_init(struct k_component *component, void *params) {


    return 0;
}

void yx_camera_target_fini(struct k_component *component) {

}

int yx_camera_manager_init(struct k_component_manager *manager, void *params) {


    return 0;
}

void yx_camera_manager_fini(struct k_component_manager *manager) {

}

static struct k_component_type *yx__camera_component_type;

int yx_camera_component_define(void) {

    struct k_component_entity_config entity_config = K_COMPONENT_ENTITY_CONFIG_INIT;
    entity_config.data_size = sizeof(struct yx_camera_target);
    entity_config.fn_init = yx_camera_target_init;
    entity_config.fn_fini = yx_camera_target_fini;

    struct k_component_manager_config manager_config = K_COMPONENT_MANAGER_CONFIG_INIT;
    manager_config.data_size = sizeof(struct yx_camera_manager);
    manager_config.fn_init = yx_camera_manager_init;
    manager_config.fn_fini = yx_camera_manager_fini;

    struct k_component_type *type = k_component_define(&manager_config, &entity_config);
    if (NULL == type)
        return -1;

    yx__camera_component_type = type;
    return 0;
}
