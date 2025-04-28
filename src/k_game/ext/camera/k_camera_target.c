#include "./k_camera_internal.h"

int k__camera_target_init(struct k_component *component, void *param) {
    float **xy = (float **)param;

    struct k_camera *camera = k_component_get_manager_data(component);
    if (NULL == camera)
        return -1;

    if (K__CAMERA_TARGET_MAX <= camera->targets_num)
        return -1;

    struct k_camera_target *target = k_component_get_data(component);
    target->component = component;
    target->x = xy[0];
    target->y = xy[1];
    target->weight = 1.0f;

    camera->targets[camera->targets_num] = target;
    camera->targets_num += 1;

    return 0;
}

void k__camera_target_fini(struct k_component *component) {
    struct k_camera_target *target = k_component_get_data(component);
    struct k_camera *camera = k_component_get_manager_data(component);

    if (target == camera->primary_target) {
        camera->primary_target = NULL;
    }

    size_t idx = 0;
    for (; idx < camera->targets_num; idx++) {
        if (camera->targets[idx] == target) {
            camera->targets[idx] = camera->targets[camera->targets_num - 1];
        }
    }

    camera->targets_num -= 1;
}

struct k_camera_target *k_camera_add_follow_target(struct k_object *object, float *x, float *y) {

    if (NULL == x || NULL == y)
        return NULL;

    float *xy[2] = { x, y };

    struct k_component *component = k_object_add_component(object, k__camera_component_type, xy);
    if (NULL == component)
        return NULL;

    return k_component_get_data(component);
}

void k_camera_del_target(struct k_camera_target *target) {

    if (NULL == target)
        return;

    if (NULL != target->component) {
        k_object_del_component(target->component);
    } else {
        /* TODO */
    }
}

int k_camera_set_primary_target(struct k_camera_target *target) {

    if (NULL == target)
        return -1;

    struct k_camera *camera = k_room_get_component_manager_data(k__camera_component_type);
    if (NULL == camera)
        return -1;

    /* TODO assert( camera follow this target )
     *
     * if (camera != k_component_get_manager_data(target->component)) {
     *     return -1;
     * }
     */

    camera->primary_target = target;
    return 0;
}

int k_camera_set_target_weight(struct k_camera_target *target, float weight) {

    if (NULL == target || weight <= 0.0f)
        return -1;

    target->weight = weight;
    return 0;
}
