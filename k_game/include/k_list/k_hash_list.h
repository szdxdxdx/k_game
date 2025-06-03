#ifndef K_HASH_LIST_H
#define K_HASH_LIST_H

#include <stddef.h>

#include "k_container_of.h"

/**
 * \brief 链表节点
 *
 * 示例：
 * ```C
 *
 * ```
 */
struct k_hash_list_node {

    /** \brief [read-only] 指向后一个节点 */
    struct k_hash_list_node *next;

    /** \brief [read-only] 指向前一个节点的 `next` 的指针 */
    struct k_hash_list_node **pprev;
};

/** \brief 侵入式哈希链表容器 */
struct k_hash_list {
    struct k_hash_list_node *first;
};

static inline void k_hash_list_init(struct k_hash_list *list) {
    list->first = NULL;
}

static inline void k_hash_list_init_all(struct k_hash_list *lists, size_t lists_num) {

    struct k_hash_list *list = lists;
    for (; list < lists + lists_num; list++) {
        k_hash_list_init(list);
    }
}

static inline void k_hash_list_add(struct k_hash_list *list, struct k_hash_list_node *node) {

    if (list->first != NULL) {
        list->first->pprev = &node->next;
    }
    node->next  = list->first;
    list->first = node;
    node->pprev = &list->first;
}

static inline void k_hash_list_node_loop(struct k_hash_list_node *node) {
    node->next  = node;
    node->pprev = &node;
}

static inline int k_hash_list_node_is_loop(struct k_hash_list_node *node) {
    return node->next == node && node->pprev == &node->next;
}

static inline void k_hash_list_remove(struct k_hash_list_node *node) {

    if (NULL != node->next) {
        node->next->pprev = node->pprev;
    }
    *(node->pprev) = node->next;
}

static inline int k_hash_list_is_empty(struct k_hash_list *list) {
    return NULL == list->first;
}

#define k_hash_list_for_each(list_, iter_) \
    iter_ = (list_)->first; \
    iter_ != NULL; \
    iter_ = iter_->next

#define k_hash_list_for_each_s(list_, iter_, next_) \
    iter_ = (list_)->first, next_ = (NULL != iter_ ? iter_->next : NULL); \
    iter_ != NULL; \
    iter_ = next_, next_ = (NULL != iter_ ? iter_->next : NULL)

#endif
