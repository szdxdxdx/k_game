#include "./k_camera_internal.h"

/* region [object_add_camera_follow] */

int k__camera_target_init(struct k_component *component, void *params) {

    struct k_camera *camera = k_component_get_manager_data(component);
    if (NULL == camera)
        return -1;

    if (K__CAMERA_SECONDARY_TARGET_MAX <= camera->secondary_targets_num)
        return -1;

    float **xy = (float **)params;

    struct k_camera_target *target = k_component_get_data(component);
    target->x = xy[0];
    target->y = xy[1];
    target->weight = 1.0f;

    size_t top_idx = camera->secondary_targets_num;
    target->target_idx = top_idx;
    camera->secondary_targets[top_idx] = target;
    camera->secondary_targets_num += 1;

    return 0;
}

void k__camera_target_fini(struct k_component *component) {
    struct k_camera_target *target_to_del = k_component_get_data(component);
    struct k_camera *camera = k_component_get_manager_data(component);

    if (target_to_del == camera->primary_target) {
        camera->primary_target = NULL;
    }
    else {
        size_t last_idx = camera->secondary_targets_num - 1;
        if (target_to_del->target_idx != last_idx) {
            struct k_camera_target *swap_target = camera->secondary_targets[last_idx];
            swap_target->target_idx = target_to_del->target_idx;
            camera->secondary_targets[target_to_del->target_idx] = swap_target;
        }
        camera->secondary_targets_num -= 1;
    }
}

struct k_camera_target *k_camera_add_follow_object(struct k_object *object, float *x, float *y) {

    if (NULL == x || NULL == y)
        return NULL;

    float *xy[2] = { x, y };

    struct k_component *component = k_object_add_component(object, k__camera_component_type, xy);
    if (NULL == component)
        return NULL;

    return k_component_get_data(component);
}

/* endregion */

int k_camera_set_primary_target(struct k_camera_target *target) {

    if (NULL == target)
        return -1;

    struct k_camera *camera = k_room_get_component_manager_data(k__camera_component_type);
    if (NULL == camera)
        return -1;

    if (camera->primary_target == target)
        return 0;

    if (NULL == camera->primary_target) {

        size_t last_idx = camera->secondary_targets_num - 1;
        if (target->target_idx != last_idx) {
            struct k_camera_target *swap_target = camera->secondary_targets[last_idx];
            swap_target->target_idx = target->target_idx;
            camera->secondary_targets[target->target_idx] = swap_target;
        }
        camera->secondary_targets_num -= 1;
    }
    else {
        struct k_camera_target *old_main_target = camera->primary_target;
        old_main_target->target_idx = target->target_idx;
        camera->secondary_targets[target->target_idx] = old_main_target;
    }

    camera->primary_target = target;
    return 0;
}

int k_camera_set_target_weight(struct k_camera_target *target, float weight) {

    if (NULL == target && weight <= 0.0f) {
        return -1;
    } else {
        target->weight = weight;
        return 0;
    }
}
