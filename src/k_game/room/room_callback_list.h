#ifndef K_GAME__ROOM_CALLBACK_LIST_H
#define K_GAME__ROOM_CALLBACK_LIST_H

#include "k_list.h"

#include "./room_callback.h"

/* 存储房间回调的链表容器
 *
 * 链表这一数据结构足以应对绝大多数场景（吗？）。
 */
struct k_room_callback_list {

    struct k_list callback_list;

    struct k_list pending_list;
};

/* 房间回调链表的结点 */
struct k_room_callback_list_item {

    struct k_list_node list_node;

    struct k_room_callback base;

    void *data;

    void (*fn_callback)(void *data);
};

void k__room_callback_list_init(struct k_room_callback_list *list);

void k__room_callback_list_clean(struct k_room_callback_list *list);

void k__room_callback_list_exec(struct k_room_callback_list *list);

#endif
