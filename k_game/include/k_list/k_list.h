#ifndef K_LIST_H
#define K_LIST_H

#include <stddef.h>

#ifndef container_of
#define container_of(p, type, member) \
    ((type *)((char *)(p) - offsetof(type, member)))
#endif

/** \brief 链表节点结构 */
struct k_list_node {

    /** \brief 指向后一个节点 */
    struct k_list_node *next;

    /** \brief 指向前一个节点 */
    struct k_list_node *prev;
};

/** \brief 侵入式双向链表容器 */
struct k_list {

    /**
     * \brief 链表头节点
     *
     * 头节点不存储数据。链表的第一个有效节点是 `head.next`，
     * 最后一个有效节点是 `head.prev`。
     */
    struct k_list_node head;
};

/**
 * \brief 将节点自环
 *
 * 可用于初始化一个未插入任何链表的独立节点。
 */
static inline void k_list_node_loop(struct k_list_node *node) {
    node->prev = node;
    node->next = node;
}

/** \brief 判断节点是否处于自环 */
static inline int k_list_node_is_loop(struct k_list_node *node) {
    return node->next == node && node->prev == node;
}

/** \brief 初始化链表 */
static inline void k_list_init(struct k_list *list) {
    k_list_node_loop(&list->head);
}

/**
 * \brief 在某个节点之后插入一个节点
 *
 * 请确保 `prev` 是某个链表中的有效节点。
 *
 * 无论 `node` 是游离的节点，还是已插入其他链表，该函数都会将其插入到 `prev` 之后，
 * 并建立 `prev` 和 `node` 之间的前后指针关系。
 * 若 `node` 不是游离的节点，会破坏 `node` 原先所在的链表的结构。
 */
static inline void k_list_insert_after(struct k_list_node *prev, struct k_list_node *node) {
    node->next = prev->next;
    prev->next->prev = node;
    prev->next = node;
    node->prev = prev;
}

/** \brief 在某个节点之前插入一个节点 */
static inline void k_list_insert_before(struct k_list_node *next, struct k_list_node *node) {
    node->prev = next->prev;
    next->prev->next = node;
    next->prev = node;
    node->next = next;
}

/** \brief 将节点插入链表头部 */
static inline void k_list_insert_head(struct k_list *list, struct k_list_node *node) {
    k_list_insert_after(&list->head, node);
}

/** \brief 将节点插入链表尾部 */
static inline void k_list_insert_tail(struct k_list *list, struct k_list_node *node) {
    k_list_insert_before(&list->head, node);
}

/**
 * \brief 从链表中移除节点
 *
 * 将 `node` 节点的前后节点相连，使得 `node` 脱离链表。
 *
 * 不要重复移除节点！虽然 `node` 的前后节点已断开与 `node` 的连接，
 * 但 `node` 仍单向持有指向前后节点的指针，若再次移除该节点，重复断链会破坏链表结构。
 *
 * 若无法保证节点只被移除一次，可在移除后调用 `k_list_node_loop()` 将节点自环。
 * 对自环节点重复执行删除操作是安全的，不会破坏链表结构。
 */
static inline void k_list_remove(struct k_list_node *node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

/** \brief 判断链表是否为空 */
static inline int k_list_is_empty(struct k_list *list) {
    return list->head.next == &list->head;
}

/**
 * \brief 获取链表第一个有效节点
 *
 * 链表的头节点不存储数据，链表的第一个有效节点是头节点的后一个节点。
 * 函数返回链表的第一个有效节点，作为遍历链表的起点。
 */
static inline struct k_list_node *k_list_get_first(struct k_list *list) {
    return list->head.next;
}

/**
 * \brief 获取链表的尾节点
 *
 * 该函数返回链表的头节点（双向循环链表的头与尾为同一个节点），作为遍历时的终止条件使用。
 * 若遍历遇到了该节点，表示已到达链表末尾。
 */
static inline struct k_list_node *k_list_tail(struct k_list *list) {
    return &list->head;
}

/**
 * \brief 遍历链表
 *
 * 本宏用于遍历链表容器，示例：
 * ```C
 * // 自定义的数据节点
 * struct my_data {
 *     struct k_list_node link; // 嵌入链表节点，作为指针域
 *     int val;                 // 节点的实际数据
 * };
 *
 * struct k_list *list = ...; // 要遍历的链表
 * struct k_list_node *iter; // 迭代器
 * for (k_list_for_each(list, iter)) { // 遍历链表
 *     struct my_data *p_data = container_of(iter, struct my_data, link); // 通过链表节点的指针，计算实际结构体的指针
 *
 *     printf("%d ", p_data->val); // 获取并打印实际节点中的数据
 * }
 * ```
 *
 * 遍历过程中不应删除链表中的节点，否则迭代器可能失效。
 */
#define k_list_for_each(list, iter) \
    iter =  k_list_get_first(list); \
    iter != k_list_tail(list);  \
    iter =  iter->next

/**
 * \brief 遍历链表
 *
 * 与 `k_list_for_each()` 类似，但使用额外变量提前保存下一个节点，
 * 可在遍历中安全地删除当前节点 `iter`，示例：
 * ```C
 * struct k_list *list = ...; // 要遍历的链表
 * struct k_list_node *iter, *next; // 迭代器
 * for (k_list_for_each_s(list, iter, next)) { // 遍历链表
 *
 *     k_list_remove(iter); // 删除当前节点
 * }
 * ```
 *
 * 遍历过程中不应删除除了 `iter` 之外的节点，否则迭代器可能失效。
 */
#define k_list_for_each_s(list, iter, next_) \
    iter =  k_list_get_first(list), next_ = iter->next; \
    iter != k_list_tail(list); \
    iter =  next_, next_ = iter->next

/**
 * \brief 获取链表的最后一个有效节点
 *
 * 函数返回链表中最后一个有效节点，与 `k_list_get_first()` 相对，用于反向遍历链表。
 */
static inline struct k_list_node *k_list_get_last(struct k_list *list) {
    return list->head.prev;
}

/**
 * \brief 获取链表的头节点
 *
 * 与 `k_list_tail()` 相对，用于反向遍历链表。
 */
static inline struct k_list_node *k_list_head(struct k_list *list) {
    return &list->head;
}

#endif
