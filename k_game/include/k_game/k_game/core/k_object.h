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

/**
 * \brief 获取对象的 ID
 *
 * 每个对象在被创建时都会被分配一个 ID。该 ID 在房间内唯一性，并按照创建顺序递增生成。
 * 即使对象被销毁，其 ID 不会立即复用。ID 可以避免出现悬垂指针问题。
 */
size_t k_object_get_id(struct k_object *object);

/**
 * \brief 通过 ID 查找对象
 *
 * 若 ID 有效且对象仍存在，则函数返回对象指针，否则返回 `NULL`。
 */
struct k_object *k_object_find_by_id(size_t id);

/* ------------------------------------------------------------------------ */

#if 1 /* <- 这部分 API 是可选的，开启后，k_object 结构体增多一个 const char *debug_info 字段，用于记录调试信息 */

#define K__OBJECT_DEBUG_INFO
struct k_object *k_object_debug_create(size_t data_size, const char *debug_info);
#define k__object_create_debug_info_(object_data_size, file, line)  "" #object_data_size " " file ": " #line
#define k__object_create_debug_info(object_data_size, file, line)  k__object_create_debug_info_(object_data_size, file, line)

/**
 * \brief 在当前房间中创建一个对象
 *
 * 此宏仅是对函数 `k_object_create()` 的简单包装，在创建对象时记录调试信息（创建该对象的代码所在的文件与行号）
 */
#define k_object_create(data_size) \
    k_object_debug_create(data_size, k__object_create_debug_info(data_size, __FILE__, __LINE__))

#endif

#endif
