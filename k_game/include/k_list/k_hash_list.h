#ifndef K_HASH_LIST_H
#define K_HASH_LIST_H

#include <stddef.h>

#include "k_container_of.h"

/* \brief 链表节点 */
struct k_hash_list_node {

    /** \brief [read-only] 指向后一个节点 */
    struct k_hash_list_node *next;

    /** \brief [read-only] 指向前一个节点的 `next` 的指针 */
    struct k_hash_list_node **pprev;
};

/**
 * \brief 侵入式哈希链表容器
 *
 * 哈希链表是侵入式的双向链表，主要用作哈希表的底层容器，用来实现哈希桶数组。
 */
struct k_hash_list {

    /**
     * \brief [read-only] 指向头节点的指针
     *
     * 链表的头节点是第一个存储数据的有效节点。
     * 若链表为空，则头节点指针为 `NULL`。
     */
    struct k_hash_list_node *first;
};

/** \brief 初始化链表 */
static inline void k_hash_list_init(struct k_hash_list *list) {
    list->first = NULL;
}

/** \brief 初始化一组链表 */
static inline void k_hash_list_init_all(struct k_hash_list *lists, size_t lists_num) {

    struct k_hash_list *list = lists;
    for (; list < lists + lists_num; list++) {
        k_hash_list_init(list);
    }
}

/**
 * \brief 将节点自环
 *
 * 可用于初始化一个尚未插入任何链表的游离节点。
 */
static inline void k_hash_list_node_loop(struct k_hash_list_node *node) {
    node->next  = node;
    node->pprev = &node;
}

/** \brief 判断节点是否处于自环 */
static inline int k_hash_list_node_is_loop(struct k_hash_list_node *node) {
    return node->next == node && node->pprev == &node->next;
}

/**
 * \brief 往链表中添加节点
 *
 * 从哈希链表的功能上看，链表中的节点顺序不重要。
 * 本函数将新节点添加在链表头部。
 *
 * 应确保 `node` 是游离的节点，否则会破坏 `node` 原先所在的链表的结构。
 */
static inline void k_hash_list_add(struct k_hash_list *list, struct k_hash_list_node *node) {

    if (list->first != NULL) {
        list->first->pprev = &node->next;
    }
    node->next  = list->first;
    list->first = node;
    node->pprev = &list->first;
}

/**
 * \brief 从链表中移除节点
 *
 * 将 `node` 节点的前后节点相连，使得 `node` 脱离链表。
 *
 * 不要重复移除节点！虽然 `node` 的前后节点已断开与 `node` 的连接，
 * 但 `node` 仍单向持有指向前后节点的指针，若再次移除该节点，重复断链会破坏链表结构。
 *
 * 若无法保证节点只被移除一次，可在移除后调用 `k_hash_list_node_loop()` 将节点自环。
 * 对自环节点重复执行删除操作是安全的，不会破坏链表结构。
 */
static inline void k_hash_list_remove(struct k_hash_list_node *node) {

    if (NULL != node->next) {
        node->next->pprev = node->pprev;
    }
    *(node->pprev) = node->next;
}

/** \brief 判断链表是否为空 */
static inline int k_hash_list_is_empty(struct k_hash_list *list) {
    return NULL == list->first;
}

/**
 * \brief 遍历链表
 *
 * 本宏用于遍历链表容器，示例：
 * ```C
 * // 自定义的数据节点
 * struct my_data {
 *     struct k_hash_list_node link; // 嵌入链表节点，作为指针域
 *     int val; // 节点的实际数据
 * };
 *
 * struct k_hash_list *list = ...; // 要遍历的链表
 * struct k_hash_list_node *iter; // 迭代器
 * for (k_hash_list_for_each(list, iter)) { // 遍历链表
 *     struct my_data *p_data = container_of(iter, struct my_data, link);
 *
 *     printf("%d ", p_data->val); // 获取并打印实际节点中的数据
 * }
 * ```
 *
 * 遍历过程中不应增删节点，否则迭代器可能失效。
 */
#define k_hash_list_for_each(list_, iter_) \
    iter_ = (list_)->first; \
    iter_ != NULL; \
    iter_ = iter_->next

/**
 * \brief 遍历链表
 *
 * 与 `k_hash_list_for_each()` 类似，但使用额外变量提前保存下一个节点，
 * 可在遍历中安全地删除当前节点 `iter`，示例：
 * ```C
 * struct k_hash_list *list = ...; // 要遍历的链表
 * struct k_hash_list_node *iter, *next; // 迭代器
 * for (k_hash_list_for_each_s(list, iter, next)) { // 遍历链表
 *
 *     k_hash_list_remove(iter); // 删除当前节点
 * }
 * ```
 *
 * 遍历过程中不应删除 `iter` 之外的节点，也不应新增节点，否则迭代器可能失效。
 */
#define k_hash_list_for_each_s(list_, iter_, next_) \
    iter_ = (list_)->first; \
    (iter_ != NULL) ? ((next_ = iter_->next), 1) : 0; \
    iter_ = next_

#endif
