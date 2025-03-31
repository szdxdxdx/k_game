#ifndef K_STR_MAP_H
#define K_STR_MAP_H

#include <stddef.h>

#include "k_str_hash_map.h"

/** \brief 哈希表容器 */
struct k_str_map {

    /** \brief [private] 内存分配函数 */
    void *(*fn_malloc)(size_t size);

    /** \brief [private] 内存释放函数 */
    void (*fn_free)(void *p);

    /** \brief [read-only] 容器持有的元素的数量 */
    size_t size;

    /** \brief [private] 使用侵入式的哈希表作为本容器的底层实现 */
    struct k_str_hash_map hash_map;

    /** \brief [private] 当元素数量达到此阈值时扩容哈希桶 */
    size_t rehash_threshold;
};

/** \brief 用于构造哈希表的配置参数 */
struct k_str_map_config {

    /** \brief 内存分配函数 */
    void *(*fn_malloc)(size_t size);

    /** \brief 内存释放函数 */
    void (*fn_free)(void *p);
};

/**
 * \brief 创建哈希表
 *
 * 若成功，函数返回哈希表容器的指针，否则返回 `NULL`。
 */
struct k_str_map *k_str_map_create(const struct k_str_map_config *config);

/**
 * \brief 销毁哈希表
 *
 * 若 `map` 为 `NULL`，函数立即返回。
 */
void k_str_map_destroy(struct k_str_map *map);

struct k_str_map *k_str_map_construct(struct k_str_map *map, const struct k_str_map_config *config);

void k_str_map_destruct(struct k_str_map *map);

/**
 * \brief 往哈希表中插入或更新一个键值对
 *
 * 若键已存在，则释放旧值的内存，然后分配新空间存储新值。
 * 若键不存在，则直接分配新空间存储新值。
 *
 * 哈希表仅保存 `key` 字符串的指针，而不会复制字符串。
 * 请确保 `key` 的内容不被修改，且生命周期要大于哈希表。
 *
 * `value_size` 指定要插入或更新的值的大小，单位：字节。
 *
 * 若成功，函数返回指向用于存储新值的内存段的指针，
 * 你需要显式类型转换该指针，然后写入新值。
 *
 * 若失败，函数返回 `NULL`。
 */
void *k_str_map_put(struct k_str_map *map, const char *key, size_t value_size);

/**
 * \brief 删除哈希表中的一个键值对
 *
 * 若 `key` 不存在，函数立即返回。
 */
void k_str_map_del(struct k_str_map *map, const char *key);

/**
 * \brief 获取哈希表中指定键对应的值
 *
 * 函数返回 `key` 对应的值的指针，若 `key` 不存在则返回 `NULL`。
 */
void *k_str_map_get(struct k_str_map *map, const char *key);

#endif
