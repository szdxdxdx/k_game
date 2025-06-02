#ifndef K_INT_INTRUSIVE_MAP_H
#define K_INT_INTRUSIVE_MAP_H

#include <stddef.h>

#include "k_hash_list.h"

/** \brief 键为 int 类型的侵入式哈希表容器 */
struct k_int_intrusive_map {

    /** \brief [read-only] 哈希桶 */
    struct k_hash_list *buckets;

    /** \brief [read-only] 哈希桶的个数 */
    size_t buckets_num;
};

/** \brief 哈希表的节点 */
struct k_int_intrusive_map_node {

    /** \brief [private] 哈希表节点的指针域 */
    struct k_hash_list_node node_link;

    /** \brief [read-only] 哈希表节点的键 */
    int key;
};

/**
 * \brief 初始化哈希表
 *
 * 初始化哈希表容器，使用外部提供的哈希桶数组。
 * 若不再使用哈希表，需自行释放该哈希桶数组的内存。
 * 示例：
 * ```C
 * size_t buckets_num = 32;
 * struct k_hash_list *buckets = malloc(sizeof(struct k_hash_list) * buckets_num);
 *
 * struct k_int_intrusive_map map;
 * k_int_intrusive_map_init(&map, buckets, buckets_num); // 初始化哈希表
 *
 * // ...
 *
 * free(map->buckets); // 不再使用哈希表，释放哈希桶数组的内存
 * ```
 */
void k_int_intrusive_map_init(struct k_int_intrusive_map *map, struct k_hash_list *buckets, size_t buckets_num);

/**
 * \brief 往哈希表中添加新节点
 *
 * 若哈希表中不存在 key 相同的节点，则添加成功，函数返回 0，否则返回非 0。
 */
int k_int_intrusive_map_add_if_absent(struct k_int_intrusive_map *map, int key, struct k_int_intrusive_map_node *node);

/**
 * \brief 往哈希表中直接添加新节点
 *
 * 直接往哈希表中添加新节点，请调用者自行确保 key 不重复。
 */
void k_int_intrusive_map_add_directly(struct k_int_intrusive_map *map, int key, struct k_int_intrusive_map_node *node);

/**
 * \brief 查找指定键的节点
 *
 * 在哈希表中查找 key 对应的节点。若找到，函数返回该节点，否则返回 `NULL`。
 */
struct k_int_intrusive_map_node *k_int_intrusive_map_get(struct k_int_intrusive_map *map, int key);

/**
 * \brief 从哈希表中移除节点
 *
 * 不需要指定 key，只要提供节点指针即可。注意不要重复移除节点。
 */
void k_int_intrusive_map_remove(struct k_int_intrusive_map_node *node);

/**
 * \brief 重新哈希
 *
 * 函数将当前哈希表中的所有节点迁移到新的桶数组中，并返回旧哈希桶数组的指针。
 * 新桶数组由调用者提供，必须在调用前分配好。
 */
struct k_hash_list *k_int_intrusive_map_rehash(struct k_int_intrusive_map *map, struct k_hash_list *new_buckets, size_t new_buckets_num);

/**
 * \brief 遍历哈希表中所有桶
 *
 * 遍历哈希表需两层循环，外层循环遍历哈希表桶数组，内层循环遍历哈希链表。
 * 本宏用于外层循环，而内层循环可使用宏 `k_hash_list_for_each()`。
 *
 * 示例：
 * ```C
 * // 假设哈希表中存储数据的结构如下：
 * struct my_data {
 *     struct k_int_intrusive_map_node map_node;
 *     int num;
 * };
 *
 * struct k_int_intrusive_map *map = ...; // 要遍历的哈希表
 *
 * struct k_hash_list *bucket;
 * for (k_int_intrusive_map_for_each_bucket(map, bucket)) { // 遍历哈希桶数组
 *
 *     struct k_hash_list_node *iter;
 *     for (k_hash_list_for_each(bucket, iter)) { // 遍历哈希链表
 *         struct my_data *data = k_int_intrusive_map_node_container_of(iter, struct my_data, map_node);
 *
 *         printf("%d", data->num);
 *     }
 * }
 * ```
 */
#define k_int_intrusive_map_for_each_bucket(map, bucket) \
    bucket = (map)->buckets; \
    bucket < (map)->buckets + (map)->buckets_num; \
    bucket++

/**
 * \brief 通过哈希链表的迭代器的指针，反推出用户自定义结构体的指针
 *
 * 用法详见宏 `k_int_intrusive_map_for_each_bucket()` 的示例。
 */
#define k_int_intrusive_map_node_container_of(hash_list_iter, type, member) \
    container_of(container_of((hash_list_iter), struct k_int_intrusive_map_node, node_link), type, member)

#endif
