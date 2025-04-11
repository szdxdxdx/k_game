#ifndef K_HASH_LIST_H
#define K_HASH_LIST_H

#include <stddef.h>

#ifndef container_of
#define container_of(p, type, member) \
    ((type *)((char *)(p) - offsetof(type, member)))
#endif

struct k_hash_list_node {
    struct k_hash_list_node *next;
    struct k_hash_list_node **pprev;
};

struct k_hash_list {
    struct k_hash_list_node *first;
};

static inline void k_hash_list_init(struct k_hash_list *list) {
    list->first = NULL;
}

static inline void k_hash_list_init_all(struct k_hash_list *lists, size_t lists_num) {

    size_t i = 0;
    for (; i < lists_num; i++) {
        k_hash_list_init(&lists[i]);
    }
}

static inline void k_hash_list_add(struct k_hash_list *list, struct k_hash_list_node *node) {

    if (list->first != NULL)
        list->first->pprev = &node->next;
    node->next  = list->first;
    list->first = node;
    node->pprev = &list->first;
}

static inline void k_hash_list_node_loop(struct k_hash_list_node *node) {
    node->next = node;
    node->pprev = &node;
}

static inline void k_hash_list_del(struct k_hash_list_node *node) {

    if (NULL != node->next)
        node->next->pprev = node->pprev;
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
