#include "./k_camera_internal.h"

#define K_LOG_TAG "k_game:camera"
#include "k_log.h"

void k_camera_set_max_speed(float max_speed) {

    struct k_camera *camera = k_room_get_component_manager_data(k__camera_component_type);
    if (NULL == camera) {
        k_log_error("please add a camera to the room first");
        return;
    }

    if (max_speed <= 0.0f) {
        k_log_error("camera max speed must be greater than 0.0f");
        return;
    }

    camera->max_speed = max_speed;
}

void k_camera_set_acceleration(float acceleration) {

    struct k_camera *camera = k_room_get_component_manager_data(k__camera_component_type);
    if (NULL == camera) {
        k_log_error("please add a camera to the room first");
        return;
    }

    if (acceleration <= 0.0f) {
        k_log_error("camera acceleration must be greater than 0.0f");
        return;
    }

    camera->acceleration = acceleration;
}
