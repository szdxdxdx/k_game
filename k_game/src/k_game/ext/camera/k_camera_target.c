
#define K_LOG_TAG "k_game:camera"
#define K_LOG_LEVEL K_LOG_LEVEL_OFF
#include "k_log.h"

#include "./k_camera_internal.h"

int k__camera_target_on_create(struct k_component *component, void *param) {
    float **xy = (float **)param;

    struct k_camera *camera = k_component_get_manager_data(component);
    if (NULL == camera) {
        k_log_error("cannot add camera follow target: please add a camera to the room first");
        return -1;
    }

    if (K__CAMERA_TARGET_MAX <= camera->targets_num) {
        k_log_error("cannot add camera follow target: follow target limit reached (%d)", K__CAMERA_TARGET_MAX);
        return -1;
    }

    struct k_camera_target *target = k_component_get_data(component);
    target->component = component;
    target->x = xy[0];
    target->y = xy[1];
    target->weight = 1.0f;

    camera->targets[camera->targets_num] = target;
    camera->targets_num += 1;

    return 0;
}

void k__camera_target_on_destroy(struct k_component *component) {
    struct k_camera_target *target = k_component_get_data(component);
    struct k_camera *camera = k_component_get_manager_data(component);

    if (target == camera->primary_target) {
        camera->primary_target = NULL;
    }

    size_t idx = 0;
    for (; idx < camera->targets_num; idx++) {
        if (camera->targets[idx] == target) {
            camera->targets[idx] = camera->targets[camera->targets_num - 1];
            break;
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

void k_camera_set_primary_target(struct k_camera_target *target) {

    struct k_camera *camera = k_room_get_component_manager_data(k__camera_component_type);
    if (NULL == camera) {
        k_log_error("please add a camera to the room first");
        return;
    }

    if (NULL == target) {
        camera->primary_target = NULL;
    }
    else {
        /* TODO assert( camera follow this target ) */
        camera->primary_target = target;
    }
}

void k_camera_set_target_weight(struct k_camera_target *target, float weight) {

    if (NULL == target) {
        k_log_error("camera target is null");
        return;
    }

    if (weight <= 0.0f) {
        k_log_error("camera target weight must be positive");
        return;
    }

    target->weight = weight;
}
