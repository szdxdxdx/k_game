#ifndef K_LIST_H
#define K_LIST_H

#include <stddef.h>

#ifndef container_of  /* 该宏用于实现“从成员指针还原到结构体指针”的操作，是侵入式容器的关键实现。若没有定义此宏，则定义 */
#define container_of(p, type, member) ((type *)((char *)(p) - offsetof(type, member)))
#endif

struct k_list_node { /* 链表节点 */
    struct k_list_node *next;
    struct k_list_node *prev;
};

struct k_list { /* 链表容器 */
    /** 链表头节点。头节点不存储数据。链表的第一个有效节点是 `head.next`，最后一个有效节点是 `head.prev` */
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

static inline void k_list_insert_after(struct k_list_node *prev, struct k_list_node *node) {
    node->next = prev->next;
    prev->next->prev = node;
    prev->next = node;
    node->prev = prev;
}

static inline void k_list_insert_before(struct k_list_node *next, struct k_list_node *node) {
    node->prev = next->prev;
    next->prev->next = node;
    next->prev = node;
    node->next = next;
}

static inline void k_list_insert_head(struct k_list *list, struct k_list_node *node) {
    k_list_insert_after(&list->head, node);
}

static inline void k_list_insert_tail(struct k_list *list, struct k_list_node *node) {
    k_list_insert_before(&list->head, node);
}

static inline void k_list_remove(struct k_list_node *node) {
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

static inline struct k_list_node *k_list_get_last(struct k_list *list) {
    return list->head.prev;
}

static inline struct k_list_node *k_list_head(struct k_list *list) {
    return &list->head;
}

/* 本宏用于遍历链表容器，示例：
 * ```C
 * // 自定义的数据节点
 * struct my_data {
 *     struct k_list_node link; // 嵌入链表节点，作为指针域
 *     int val;                 // 节点的实际数据
 * };
 *
 * struct k_list *list = ...; // 要遍历的链表
 * struct k_list_node *p_node; // 定义用于迭代的遍历
 * for (k_list_for_each(list, p_node)) { // 遍历链表
 *     struct my_data *p_data = container_of(p_node, struct my_data, link); // 通过链表节点的指针，计算实际结构体的指针
 *
 *     printf("%d\n", p_data->val); // 获取并打印实际节点中的数据
 * }
 * ```
 */
#define k_list_for_each(list_, iter_) \
    iter_ = (list_)->head.next; \
    iter_ != &((list_)->head);  \
    iter_ = iter_->next

#define k_list_reversed_for_each(list_, iter_) \
    iter_ = (list_)->head.prev; \
    iter_ != &((list_)->head);  \
    iter_ = iter_->prev

#define k_list_for_each_s(list_, iter_, next_) \
    iter_ = (list_)->head.next, next_ = iter_->next; \
    iter_ != &((list_)->head); \
    iter_ = next_, next_ = iter_->next

#endif
