
#include "./yx_comp_camera.h"

struct yx_camera;

struct yx_camera_target {

    struct yx_camera *camera;

    size_t target_idx;

    float *x;
    float *y;
};

struct yx_camera_manager {

    struct k_object *camera;
};

struct yx_camera {

    struct yx_camera_manager *manager;

    float camera_x;
    float camera_y;

#define YX__CAMERA_TARGET_MAX 16
    struct yx_camera_target *targets[YX__CAMERA_TARGET_MAX];

    size_t targets_num;
};

int yx_camera_target_init(struct k_component *component, void *params) {

    struct yx_camera *camera = k_component_get_manager_data(component);
    if (NULL == camera)
        return -1;

    if (YX__CAMERA_TARGET_MAX <= camera->targets_num)
        return -1;

    struct yx_camera_target *target = k_component_get_data(component);

    target->camera = camera;
    target->target_idx = camera->targets_num;

    float **xy = (float **)params;
    target->x = xy[0];
    target->y = xy[1];

    camera->targets_num += 1;

    return 0;
}

void yx_camera_target_fini(struct k_component *component) {
    struct yx_camera_target *target = k_component_get_data(component);

    struct yx_camera *camera = target->camera;

    struct yx_camera_target *last_target = camera->targets[camera->targets_num - 1];
    if (target != last_target) {
        last_target->target_idx = target->target_idx;
        camera->targets[target->target_idx] = last_target;
    }
    camera->targets_num -= 1;
}

int yx_camera_manager_init(struct k_component_manager *component_manager, void *params) {
    (void *)params;

    struct yx_camera_manager *manager = k_component_manager_get_data(component_manager);

    {
        struct k_object *object = k_object_create(sizeof(struct yx_camera));
        if (NULL == object)
            return -1;

        struct yx_camera *camera = k_object_get_data(object);
        camera->manager     = manager;
        camera->camera_x    = 0.0f;
        camera->camera_y    = 0.0f;
        camera->targets_num = 0;

        manager->camera = object;
    }

    return 0;
}

void yx_camera_manager_fini(struct k_component_manager *component_manager) {
    struct yx_camera_manager *manager = k_component_manager_get_data(component_manager);

    (void)manager;
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

void yx_room_add_camera_manager(void) {
    k_room_add_component_manager(yx__camera_component_type, NULL);
}

int yx_object_add_camera_follow(struct k_object *object, float *x, float *y) {

    float *xy[2] = { x, y };

    struct k_component *component = k_object_add_component(object, yx__camera_component_type, xy);
    if (NULL == component)
        return -1;
    else
        return 0;
}
