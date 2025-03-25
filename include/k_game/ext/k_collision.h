#ifndef K_COLLISION_H
#define K_COLLISION_H

struct k_float_vec2;
struct k_room;
struct k_object;

/**
 * TODO Docs
 */
struct k_collision_box;

/* region [room_add_collision_box_manager] */

int k_room_add_collision_manager(void);

/* endregion */

/* region [object_add_collision_box] */

/* region [point] */

struct k_collision_point_config {

    int group_id;

    float *x;
    float *y;

    float offset_x;
    float offset_y;
};

struct k_collision_box *k_object_add_collision_point(struct k_object *object, const struct k_collision_point_config *config);

/* endregion */

/* region [line] */

struct k_collision_line_config {

    int group_id;

    float *x;
    float *y;

    float offset_x1;
    float offset_y1;
    float offset_x2;
    float offset_y2;
};

struct k_collision_box *k_object_add_collision_line(struct k_object *object, const struct k_collision_line_config *config);

/* endregion */

/* region [rectangle] */

struct k_collision_rectangle_config {

    int group_id;

    float *x;
    float *y;

    float offset_x1;
    float offset_y1;
    float offset_x2;
    float offset_y2;
};

/**
 * \brief 给对象挂载一个矩形的碰撞盒组件
 *
 * 若成功，函数返回碰撞盒组件的指针，否则返回 `NULL`。
 */
struct k_collision_box *k_object_add_collision_rectangle(struct k_object *object, const struct k_collision_rectangle_config *config);

/* endregion */

/* region [circle] */

struct k_collision_circle_config {

    int group_id;

    float *x;
    float *y;

    float offset_cx;
    float offset_cy;
    float r;
};

/**
 * \brief 给对象挂载一个圆形的碰撞盒组件
 *
 * 若成功，函数返回碰撞盒组件的指针，否则返回 `NULL`。
 */
struct k_collision_box *k_object_add_collision_circle(struct k_object *object, const struct k_collision_circle_config *config);

/* endregion */

/**
 * \brief 移除对象上挂载的碰撞盒组件
 *
 * 若 `box` 为 `NULL`，则函数不做任何事。
 */
void k_object_del_collision_box(struct k_collision_box *box);

/* endregion */

/* region [collision_box_get] */

/**
 * \brief 获取碰撞盒关联的对象
 *
 * 函数返回挂载了该碰撞盒的对象。
 */
struct k_object *k_collision_box_get_object(struct k_collision_box *box);

/* endregion */

/* region [collision_check] */

/**
 * \brief 检测是否有碰撞盒与指定的点相交
 *
 * 函数检查 `group_id` 的碰撞组中是否存在与点 `(x, y)` 相交的碰撞盒，
 * 若存在，函数返回找到的第一个碰撞盒，否则返回 `NULL`。
 */
struct k_collision_box *k_collision_check_point(int group_id, float x, float y);

/**
 * \brief 检测是否有碰撞盒与指定的线段相交
 *
 * 由两端点坐标为 `(x1, y1)` 和 `(x2, y2)` 定义一条线段，
 * 函数检查 `group_id` 的碰撞组中是否存在与该线段相交的碰撞盒，
 * 若存在，函数返回找到的第一个碰撞盒，否则返回 `NULL`。
 */
struct k_collision_box *k_collision_check_line(int group_id, float x1, float y1, float x2, float y2);

/**
 * \brief 检测是否有碰撞盒与指定的矩形相交
 *
 * 由对角坐标 `(x1, y1)` 和 `(x2, y2)` 定义一个矩形区域，
 * 函数检查 `group_id` 的碰撞组中是否存在与该区域相交的碰撞盒，
 * 若存在，函数返回找到的第一个碰撞盒，否则返回 `NULL`。
 */
struct k_collision_box *k_collision_check_rectangle(int group_id, float x1, float y1, float x2, float y2);

/**
 * \brief 检测是否有碰撞盒与指定的圆相交
 *
 * 由圆心坐标 `(x, y)` 及半径 `r` 定义一个圆形区域，
 * 函数检查 `group_id` 的碰撞组中是否存在与该区域相交的碰撞盒，
 * 若存在，函数返回找到的第一个碰撞盒，否则返回 `NULL`。
 */
struct k_collision_box *k_collision_check_circle(int group_id, float x, float y, float r);

/* endregion */

#endif
