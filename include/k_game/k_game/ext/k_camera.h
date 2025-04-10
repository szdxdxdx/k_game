#ifndef K_CAMERA_H
#define K_CAMERA_H

struct k_object;

/**
 * \brief 摄像机
 *
 */
struct k_camera;

struct k_camera_target;

/**
 * \brief 给当前房间添加摄像机
 *
 */
int k_room_add_camera(void);

int k_camera_set_max_speed(float speed);

int k_camera_set_acceleration(float acceleration);

struct k_camera_target *k_camera_add_follow_object(struct k_object *object, float *x, float *y);

struct k_camera_target *k_camera_add_follow_target(float *x, float *y);

void k_camera_del_target(struct k_camera_target *target);

/**
 * \brief 设置摄像机要跟随的主对象
 *
 */
int k_camera_set_primary_target(struct k_camera_target *target);

int k_camera_set_target_weight(struct k_camera_target *target, float weight);

#endif
