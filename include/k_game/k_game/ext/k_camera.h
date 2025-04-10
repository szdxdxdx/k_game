#ifndef K_CAMERA_H
#define K_CAMERA_H

struct k_object;

/**
 * \brief 摄像机
 *
 * 摄像机用于控制视角的平滑移动与目标跟随，
 * 能根据多个跟随目标位置动态调整视野中心。
 *
 * k_camera 的本质其实是 k_component_manager。
 */
struct k_camera;

/** \brief 摄像机的跟随目标 */
struct k_camera_target;

/* region [room_add_camera] */

/**
 * \brief 给当前房间添加摄像机
 *
 * 房间最多只能有一个摄像机。
 *
 * 若添加成功，函数返回 0，否则返回非 0。
 */
int k_room_add_camera(void);

/* endregion */

/* region [camera_target] */

/**
 * \brief 向摄像机添加跟随的对象
 *
 * 若添加成功，函数返回跟随目标的指针，否则返回 `NULL`。
 */
struct k_camera_target *k_camera_add_follow_object(struct k_object *object, float *x, float *y);

/* TODO */
struct k_camera_target *k_camera_add_follow_target(float *x, float *y);

/**
 * \brief 删除摄像机的跟随目标
 *
 * 若 `target` 为 `NULL`，则函数立即返回。
 */
void k_camera_del_target(struct k_camera_target *target);

/**
 * \brief 指定摄像机的主跟随目标
 *
 * 若设置了主跟随目标，摄像机会尽量保证主目标在视野范围内，
 * 视野的中心坐标由视野内的所有跟随目标加权平均决定。
 * 若主跟随目标移动过快，摄像机跟不上，仍可能暂时脱离视野。
 *
 * 若未设置主跟随目标，视野中心由所有跟随目标加权平均决定。
 *
 * 最多只能有一个主跟随目标，每次调用都将指定新的主跟随目标，
 * 若 `target` 为 `NULL` 则取消跟随主目标。
 *
 * 若设置成功，函数返回 0，否则返回非 0。
 */
int k_camera_set_primary_target(struct k_camera_target *target);

/**
 * \brief 设置摄像机跟随目标的权重
 *
 * 跟随目标的权重越大，视野中心越靠近该目标。
 *
 * 权重必须为正值。
 * 设置成功返回 0，失败返回非 0。
 */
int k_camera_set_target_weight(struct k_camera_target *target, float weight);

/* endregion */

/* region [camera_speed] */

/** \brief 设置摄像机的最大移动速度，单位：像素/秒
 *
 * 最大移动速度必须为正值。
 * 若设置成功，函数返回 0，否则返回非 0。
 */
int k_camera_set_max_speed(float max_speed);

/**
 * \brief 设置摄像机的加速度，单位：像素/秒^2
 *
 * 加速度必须为正值。
 * 若设置成功，函数返回 0，否则返回非 0。
 */
int k_camera_set_acceleration(float acceleration);

/* endregion */

#endif
