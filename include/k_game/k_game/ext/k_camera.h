#ifndef K_CAMERA_H
#define K_CAMERA_H

struct k_object;

struct k_camera_target;

int k_room_add_camera(void);

struct k_camera_target *k_camera_add_follow_object(struct k_object *object, float *x, float *y);

struct k_camera_target *k_camera_add_follow_target(float *x, float *y);

void k_camera_del_target(struct k_camera_target *target);

int k_camera_set_primary_target(struct k_camera_target *target);

int k_camera_set_target_weight(struct k_camera_target *target, float weight);

int k_camera_set_max_speed(float speed);

int k_camera_set_acceleration(float acceleration);

#endif
