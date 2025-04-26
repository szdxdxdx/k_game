#ifndef K_STR_MAP_H
#define K_STR_MAP_H

#include <stddef.h>

#include "k_hash_list.h"

/** \brief 哈希表容器 */
struct k_str_map {

    /** \brief [private] 内存分配函数 */
    void *(*fn_malloc)(size_t size);

    /** \brief [private] 内存释放函数 */
    void (*fn_free)(void *p);

    /** \brief [read-only] 容器持有的元素的数量 */
    size_t size;

    /** \brief [private] 哈希桶数组 */
    struct k_hash_list *buckets;

    /** \brief [private] 哈希桶的数量 */
    size_t buckets_num;

    /** \brief [private] 扩容阈值，当元素数量超过阈值时扩容哈希桶 */
    size_t rehash_threshold;
};

/** \brief 用于构造哈希表的可选配置参数 */
struct k_str_map_options {

    /** \brief 内存分配函数 */
    void *(*fn_malloc)(size_t size);

    /** \brief 内存释放函数 */
    void (*fn_free)(void *p);
};

/**
 * \brief 创建哈希表
 *
 * `options` 是可选的配置，若为 `NULL` 则使用默认配置。
 *
 * 若创建成功，函数返回哈希表的指针，否则返回 `NULL`。
 */
struct k_str_map *k_str_map_create(const struct k_str_map_options *options);

/**
 * \brief 销毁哈希表
 *
 * 若 `map` 为 `NULL`，则函数立即返回。
 */
void k_str_map_destroy(struct k_str_map *map);

/**
 * \brief 构造哈希表
 *
 * 在 `map` 所指向的内存段上原地构造哈希表。
 * `options` 是可选的配置，若为 `NULL` 则使用默认配置。
 * 
 * 若构造成功，函数返回值同入参 `map`，否则返回 `NULL`。
 */
struct k_str_map *k_str_map_construct(struct k_str_map *map, const struct k_str_map_options *options);

/**
 * \brief 析构哈希表
 *
 * 原地析构 `map` 所指向的内存段上的哈希表。
 * 若 `map` 为 `NULL`，则函数立即返回。
 */
void k_str_map_destruct(struct k_str_map *map);

/**
 * \brief 往哈希表中添加或更新一个键值对
 *
 * 若键不存在，则添加新键值对。
 * 若键已存在，则释放旧值的内存，然后分配新内存来存储新值。
 *
 * `key` 是要添加或更新的键，要求传入有效的字符串指针，且字符串非空，
 * 哈希表会复制保存 `key` 字符串。
 *
 * `val_size` 指定要添加或更新的值的大小，单位：字节。
 * 要求 `val_size` 大于 0。
 *
 * 若添加或更新成功，函数返回指向用于存储新值的内存段的指针，
 * 你需要显式类型转换该指针，然后写入新值。若添加失败，函数返回 `NULL`。
 */
void *k_str_map_put(struct k_str_map *map, const char *key, size_t val_size);

/**
 * \brief 往哈希表中添加或更新一个键值对
 *
 * 与 `k_str_map_put()` 类似，但哈希表仅保存 `key` 指针，不复制字符串。
 * 请保证 `key` 字符串的生命周期大于哈希表，且内容不被修改，
 * 否则可能导致哈希表内部的指针悬空，或是查找时哈希不一致等问题。
 */
void *k_str_map_put_ref(struct k_str_map *map, const char *key, size_t val_size);

/**
 * \brief 往哈希表中添加一个新的键值对
 *
 * 若键不存在，则添加新键值对。若键已存在，则添加失败。
 *
 * `key` 是要添加的键，要求传入有效的字符串指针，且字符串非空，
 * 哈希表会复制保存 `key` 字符串。
 *
 * `val_size` 指定要添加的值的大小，单位：字节。
 * 要求 `val_size` 大于 0。
 *
 * 若添加成功，函数返回指向用于存储新值的内存段的指针，
 * 你需要显式类型转换该指针，然后写入新值。若添加失败，函数返回 `NULL`。
 */
void *k_str_map_add(struct k_str_map *map, const char *key, size_t val_size);

/**
 * \brief 往哈希表中添加或更新一个键值对
 *
 * 与 `k_str_map_add()` 类似，但哈希表仅保存 `key` 指针，不复制字符串。
 * 请保证 `key` 字符串的生命周期大于哈希表，且内容不被修改，
 * 否则可能导致哈希表内部的指针悬空，或是查找时哈希不一致等问题。
 */
void *k_str_map_add_ref(struct k_str_map *map, const char *key, size_t val_size);

/**
 * \brief 删除哈希表中的一个键值对
 *
 * 若 `key` 不存在，则函数立即返回。
 */
void k_str_map_del(struct k_str_map *map, const char *key);

/**
 * \brief 获取哈希表中指定键对应的值
 *
 * 函数返回 `key` 对应的值的指针，若 `key` 不存在则返回 `NULL`。
 */
void *k_str_map_get(struct k_str_map *map, const char *key);

/**
 * \brief 清空哈希表
 *
 * 删除哈希表中所有的键值对，并保留哈希桶数组的当前容量。
 */
void k_str_map_clear(struct k_str_map *map);

#endif
