#ifndef K_LIST_H
#define K_LIST_H

#include <stddef.h>

#ifndef container_of
#define container_of(p, type, member) \
    ((type *)((char *)(p) - offsetof(type, member)))
#endif

struct k_list_node {
    struct k_list_node *next;
    struct k_list_node *prev;
};

struct k_list {

    /**
     * \brief 链表头节点
     *
     * 头节点不存储数据。链表的第一个有效节点是 `head.next`，
     * 最后一个有效结点是 `head.prev`。
     */
    struct k_list_node head;
};

static inline void k_list_node_loop(struct k_list_node *node) {
    node->prev = node;
    node->next = node;
}

static inline int k_list_node_is_loop(struct k_list_node *node) {
    return node->next == node && node->prev == node;
}

static inline void k_list_init(struct k_list *list) {
    k_list_node_loop(&list->head);
}

static inline void k_list_add(struct k_list_node *prev, struct k_list_node *node) {
    node->next = prev->next;
    prev->next->prev = node;
    prev->next = node;
    node->prev = prev;
}

static inline void k_list_add_head(struct k_list *list, struct k_list_node *node) {
    k_list_add(&list->head, node);
}

static inline void k_list_add_tail(struct k_list *list, struct k_list_node *node) {
    k_list_add(list->head.prev, node);
}

static inline void k_list_del(struct k_list_node *node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

static inline int k_list_is_empty(struct k_list *list) {
    return list->head.next == &list->head;
}

static inline struct k_list_node *k_list_get_first(struct k_list *list) {
    return list->head.next;
}

static inline struct k_list_node *k_list_tail(struct k_list *list) {
    return &list->head;
}

#define k_list_for_each(list_, iter_) \
    iter_ = (list_)->head.next; \
    iter_ != &((list_)->head); \
    iter_ = iter_->next

#define k_list_reversed_for_each(list_, iter_) \
    iter_ = (list_)->head.prev; \
    iter_ != &((list_)->head); \
    iter_ = iter_->prev

#define k_list_for_each_s(list_, iter_, next_) \
    iter_ = (list_)->head.next, next_ = iter_->next; \
    iter_ != &((list_)->head); \
    iter_ = next_, next_ = iter_->next

#endif
