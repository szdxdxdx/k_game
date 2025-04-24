#ifndef K_OBJECT_H
#define K_OBJECT_H

#include <stddef.h>

#include "./k_game_fwd.h"

/**
 * \brief 在当前房间中创建一个对象
 *
 * `data_size` 指定对象的自定义关联数据的结构体大小，单位：字节。
 * 该内存由 k_game 负责分配与释放，其生命周期与对象一致。
 * 可通过 `k_object_get_data()` 访问该内存。
 *
 * 若 `data_size` 为 0 则不附带关联数据，
 * 之后 `k_object_get_data()` 返回 `NULL`。
 *
 * 若创建成功则函数返回对象的指针，失败则返回 `NULL`。
 */
struct k_object *k_object_create(size_t data_size);

/**
 * \brief 销毁对象
 *
 * 若 `object` 为 `NULL` 则函数立即返回。
 */
void k_object_destroy(struct k_object *object);

/** \brief 获取对象的关联数据 */
void *k_object_get_data(struct k_object *object);

#endif
