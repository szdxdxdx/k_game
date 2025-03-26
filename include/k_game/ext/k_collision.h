#ifndef K_COLLISION_H
#define K_COLLISION_H

struct k_room;
struct k_object;

/**
 * \brief 碰撞盒
 *
 * 碰撞盒的本质是一个配有管理器的组件。
 * 需要先给房间添加碰撞盒管理器，房间内的对象才能挂载碰撞盒。
 *
 * 碰撞盒使对象能感知彼此的存在，从而实现交互。
 *
 * 例如：若子弹和敌人都是对象，要实现子弹击中敌人的效果，
 * 可以给敌人挂载碰撞盒，在子弹移动过程中，检测自身是否击中敌人；
 * 或给子弹挂载碰撞盒，在敌人移动过程中，检测自身是否被子弹击中。
 */
struct k_collision_box;

/* region [room_add_collision_box_manager] */

/**
 * \brief 给房间添加碰撞盒管理器
 *
 * 需要先给房间添加碰撞盒管理器，房间内的对象才能挂载碰撞盒。
 *
 * 若成功，函数返回非 0，否则返回 0。
 */
int k_room_add_collision_manager(void);

/* endregion */

/* region [object_add_collision_box] */

/* region [point] */

/** \brief 点碰撞盒的配置 */
struct k_collision_point_config {

    /**
     * \brief 碰撞组
     *
     * 通过指定不同的 `group_id` 来给碰撞盒分组，
     * 碰撞检测时，只检测指定组内的碰撞盒。
     *
     * `group_id` 可以是任意整数。
     */
    int group_id;

    /**
     * \brief 碰撞盒关联的坐标
     *
     * 碰撞盒通过指针关联一个外部的坐标。
     * 碰撞盒的位置会随着外部坐标的变化而自动更新。
     *
     * 碰撞检测时，将根据关联坐标计算出碰撞盒的实际位置。
     */
    float *x;
    float *y;

    /** \brief 点碰撞盒相对于关联坐标的偏移量 */
    float offset_x;
    float offset_y;
};

/**
 * \brief 给对象挂载一个点碰撞盒
 *
 * 若成功，函数返回碰撞盒的指针，否则返回 `NULL`。
 */
struct k_collision_box *k_object_add_collision_point(struct k_object *object, const struct k_collision_point_config *config);

/* endregion */

/* region [line] */

/** \brief 线段碰撞盒的配置 */
struct k_collision_line_config {

    /** \brief 碰撞组 */
    int group_id;

    /** \brief 碰撞盒关联的坐标 */
    float *x;
    float *y;

    /**
     * \brief 线段端点坐标相对于关联坐标的偏移量
     *
     * 若线段的端点重合，则退化成一个点。
     */
    float offset_x1;
    float offset_y1;
    float offset_x2;
    float offset_y2;
};

/**
 * \brief 给对象挂载一个线段碰撞盒
 *
 * 若成功，函数返回碰撞盒的指针，否则返回 `NULL`。
 */
struct k_collision_box *k_object_add_collision_line(struct k_object *object, const struct k_collision_line_config *config);

/* endregion */

/* region [rectangle] */

/** \brief 矩形碰撞盒的配置 */
struct k_collision_rectangle_config {

    /** \brief 碰撞组 */
    int group_id;

    /** \brief 碰撞盒关联的坐标 */
    float *x;
    float *y;

    /**
     * \brief 矩形对角坐标相对于关联坐标的偏移量
     *
     * 矩形的对角坐标，可以是左上角与右下角的坐标，也可以是左下角与右上角的坐标。
     * 若矩形的对边或对角重合，则退化成一条线段或一个点。
     */
    float offset_x1;
    float offset_y1;
    float offset_x2;
    float offset_y2;
};

/**
 * \brief 给对象挂载一个矩形碰撞盒
 *
 * 若成功，函数返回碰撞盒的指针，否则返回 `NULL`。
 */
struct k_collision_box *k_object_add_collision_rectangle(struct k_object *object, const struct k_collision_rectangle_config *config);

/* endregion */

/* region [circle] */

/** \brief 圆形碰撞盒的配置 */
struct k_collision_circle_config {

    /** \brief 碰撞组 */
    int group_id;

    /** \brief 碰撞盒关联的坐标 */
    float *x;
    float *y;

    /** \brief 圆心坐标相对于关联坐标的偏移量 */
    float offset_cx;
    float offset_cy;

    /** \brief 圆的半径
     *
     * 若半径为 0.0f 或负值，则退化成一个点。
     */
    float r;
};

/**
 * \brief 给对象挂载一个圆形碰撞盒
 *
 * 若成功，函数返回碰撞盒的指针，否则返回 `NULL`。
 */
struct k_collision_box *k_object_add_collision_circle(struct k_object *object, const struct k_collision_circle_config *config);

/* endregion */

/**
 * \brief 移除对象上挂载的碰撞盒
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
 * 指定一条线段，其端点坐标为 `(x1, y1)` 和 `(x2, y2)`，
 * 函数检查 `group_id` 的碰撞组中是否存在与该线段相交的碰撞盒，
 * 若存在，函数返回找到的第一个碰撞盒，否则返回 `NULL`。
 *
 * 若线段的端点重合，则检测区域退化成一个点。
 */
struct k_collision_box *k_collision_check_line(int group_id, float x1, float y1, float x2, float y2);

/**
 * \brief 检测是否有碰撞盒与指定的矩形相交
 *
 * 指定一个矩形区域，其对角坐标为 `(x1, y1)` 和 `(x2, y2)`，
 * 函数检查 `group_id` 的碰撞组中是否存在与该区域相交的碰撞盒，
 * 若存在，函数返回找到的第一个碰撞盒，否则返回 `NULL`。
 *
 * 矩形的对角坐标，可以是左上角与右下角的坐标，也可以是左下角与右上角的坐标。
 * 若矩形的对边或对角重合，则检测区域退化成一条线段或一个点。
 */
struct k_collision_box *k_collision_check_rectangle(int group_id, float x1, float y1, float x2, float y2);

/**
 * \brief 检测是否有碰撞盒与指定的圆相交
 *
 * 指定一个圆形区域，圆心坐标为 `(x, y)`，半径为 `r`，
 * 函数检查 `group_id` 的碰撞组中是否存在与该区域相交的碰撞盒，
 * 若存在，函数返回找到的第一个碰撞盒，否则返回 `NULL`。
 *
 * 若圆的半径为 0.0f 或负值，则检测区域退化成一个点。
 */
struct k_collision_box *k_collision_check_circle(int group_id, float x, float y, float r);

/* endregion */

/* region [collision_query] */

size_t k_collision_query_rectangle(struct k_collision_box **result, size_t n, int group_id, float x1, float y1, float x2, float y2);

/* endregion */

/* region [debug] */

/**
 * \brief 开启或关闭指定碰撞组的调试显示功能
 *
 * 若 `debug` 为非 0 则启用调试，否则关闭。
 * 启用调试后，将绘制 `group_id` 碰撞组内所有碰撞盒。
 *
 * 若成功，函数返回 0，否则返回非 0。
 */
int k_collision_set_debug(int group_id, int debug);

/* endregion */

#endif
