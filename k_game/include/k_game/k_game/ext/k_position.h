#ifndef K_POSITION_H
#define K_POSITION_H

#include "k_list.h"

#include "k_game/core/k_game_fwd.h"

/**
 * \brief 坐标跟随组件
 *
 * 本组件用于在多个对象之间建立“相对位置”的跟随关系。
 *
 * 例如，建立起“人物角色”、“武器”、“气泡特效”等多个游戏对象间的相对位置关系，
 * 使得角色本体移动时，其他关联对象能够始终保持相对位置不变，实现组合效果。
 */
struct k_position;

struct k_position_config {

    /** \brief 组件关联的外部坐标，是对象在房间中的实际坐标 */
    float *world_x;
    float *world_y;

    /** \brief 跟随的父坐标组件，若为 `NULL` 则无跟随效果 */
    struct k_position *parent;

    /** \brief 相对于父组件的位置偏移 */
    float local_x;
    float local_y;
};

/**
 * \brief 为对象添加坐标组件
 *
 * 若成功，函数返回组件的指针，否则返回 `NULL`。
 */
struct k_position *k_object_add_position(struct k_object *object, const struct k_position_config *config);

/**
 * \brief 移除坐标组件
 *
 * 若 `position` 为 `NULL`，则函数立即返回。
 *
 * 移除该组件时，会将子组件挂到当前组件的父组件上（即“上提一层”），
 * 维持子组件在房间中的坐标不变。
 */
void k_object_del_position(struct k_position *position);

/**
 * \brief 设置组件相对于父组件的坐标
 *
 * 设置组件相对于父组件的坐标，同时更新子组件的坐标。
 */
void k_position_set_local_position(struct k_position *position, float local_x, float local_y);
// TODO void k_position_set_offset_xy(struct k_position *position, float offset_x, float offset_y);

/**
 * \brief 设置组件相对于父组件的坐标
 *
 * 设置组件在房间中的坐标，同时更新子组件的坐标。
 */
void k_position_set_world_position(struct k_position *position, float world_x, float world_y);
// TODO void k_position_set_xy(struct k_position *position, float x, float y);

#endif
