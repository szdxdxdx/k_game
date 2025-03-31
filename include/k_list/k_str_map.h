#ifndef K_STR_MAP_H
#define K_STR_MAP_H

#include <stddef.h>

#include "k_str_hash_map.h"

/** \brief 哈希表容器（键为字符串，值为任意结构体类型） */
struct k_str_map {

    /** \brief 内存分配函数 */
    void *(*fn_malloc)(size_t size);

    /** \brief 内存释放函数 */
    void (*fn_free)(void *p);

    /** \brief 容器持有的元素的数量 */
    size_t size;

    /** \brief 使用侵入式的字符串哈希表作为本容器的底层实现 */
    struct k_str_hash_map hash_map;

    size_t rehash_threshold;
};

struct k_str_map_config {

    void *(*fn_malloc)(size_t size);

    void (*fn_free)(void *p);
};

struct k_str_map *k_str_map_create(const struct k_str_map_config *config);

void k_str_map_destroy(struct k_str_map *map);

struct k_str_map *k_str_map_construct(struct k_str_map *map, const struct k_str_map_config *config);

void k_str_map_destruct(struct k_str_map *map);

void *k_str_map_put(struct k_str_map *map, const char *key, size_t value_size);

void k_str_map_del(struct k_str_map *map, const char *key);

void *k_str_map_get(struct k_str_map *map, const char *key);

#endif
