#ifndef K_LIST_H
#define K_LIST_H

#include <stddef.h>

struct k_list_node {
    struct k_list_node *next;
    struct k_list_node *prev;
};

struct k_list {
    struct k_list_node head;
};

static inline void k_list_link(struct k_list_node *prev, struct k_list_node *next) {
    prev->next = next;
    next->prev = prev;
}

static inline void k_init_list(struct k_list *list) {
    k_list_link(&list->head, &list->head);
}

static inline void k_list_add(struct k_list_node *prev, struct k_list_node *node) {
    k_list_link(node, prev->next);
    k_list_link(prev, node);
}

static inline void k_list_add_head(struct k_list *list, struct k_list_node *node) {
    k_list_add(&list->head, node);
}

static inline void k_list_add_tail(struct k_list *list, struct k_list_node *node) {
    k_list_add(list->head.prev, node);
}

static inline void k_list_del(struct k_list_node *node) {
    k_list_link(node->prev, node->next);
}

static inline int k_list_is_empty(struct k_list *list) {
    return list->head.next == &list->head;
}

#define k_list_for_each(list_, node_) \
    node_ = (list_)->head.next; \
    node_ != &((list_)->head); \
    node_ = node_->next

#define k_list_reversed_for_each(list_, node_) \
    node_ = (list_)->head.prev; \
    node_ != &((list_)->head); \
    node_ = node_->prev

#define k_list_for_each_s(list_, node_, next_) \
    node_ = (list_)->head.next, next_ = node_->next; \
    node_ != &((list_)->head); \
    node_ = next_, next_ = node_->next

static inline struct k_list_node *k_list_get_nth(struct k_list *list, size_t n) {

    struct k_list_node *node;
    for (k_list_for_each(list, node)) {
        if (0 == n--)
            return node;
    }

    return NULL;
}

static inline struct k_list_node *k_list_reversed_get_nth(struct k_list *list, size_t n) {

    struct k_list_node *node;
    for (k_list_reversed_for_each(list, node)) {
        if (0 == n--)
            return node;
    }

    return NULL;
}

#endif
