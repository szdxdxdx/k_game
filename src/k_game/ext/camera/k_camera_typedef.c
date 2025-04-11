#include "k_game/core/k_object.h"
#include "k_game/core/k_component.h"
#include "k_game/core/k_callback.h"

#include "./k_camera_internal.h"
#include "./k_camera_typedef.h"

int k_camera_init(struct k_component_manager *manager, void *params) {
    (void)params;

    struct k_camera *camera = k_component_manager_get_data(manager);

    camera->state = K__CAMERA_AUTO_FOLLOW;

    camera->cb_camera_move = k_component_manager_add_step_end_callback(manager, camera, k__camera_move);
    if (NULL == camera->cb_camera_move)
        return -1;

    camera->max_speed    = 512.0f;
    camera->acceleration = 256.0f;
    camera->vx    = 0.0f;
    camera->vy    = 0.0f;
    camera->dst_x = 0.0f;
    camera->dst_y = 0.0f;

    camera->primary_target = NULL;
    camera->targets[0]     = NULL;
    camera->targets_num    = 0;

    camera->cb_camera_debug = NULL;

    return 0;
}

struct k_component_type *k__camera_component_type;

int k__camera_component_define(void) {

    struct k_component_entity_config entity_config = K_COMPONENT_ENTITY_CONFIG_INIT;
    entity_config.data_size = sizeof(struct k_camera_target);
    entity_config.fn_init = k__camera_target_init;
    entity_config.fn_fini = k__camera_target_fini;

    struct k_component_manager_config manager_config = K_COMPONENT_MANAGER_CONFIG_INIT;
    manager_config.data_size = sizeof(struct k_camera);
    manager_config.fn_init = k_camera_init;

    struct k_component_type *type = k_component_define(&manager_config, &entity_config);
    if (NULL == type)
        return -1;

    k__camera_component_type = type;
    return 0;
}

int k_room_add_camera(void) {
    return k_room_add_component_manager(k__camera_component_type, NULL);
}
