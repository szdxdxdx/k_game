#include "./k_camera_internal.h"

int k_camera_set_max_speed(float max_speed) {

    if (max_speed <= 0.0f)
        return -1;

    struct k_camera *camera = k_room_get_component_manager_data(k__camera_component_type);
    if (NULL == camera)
        return -1;

    camera->max_speed = max_speed;
    return 0;
}

int k_camera_set_acceleration(float acceleration) {

    if (acceleration <= 0.0f)
        return -1;

    struct k_camera *camera = k_room_get_component_manager_data(k__camera_component_type);
    if (NULL == camera)
        return -1;

    camera->acceleration = acceleration;
    return 0;
}
