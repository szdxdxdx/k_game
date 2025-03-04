#ifndef K_GAME__ROOM_CALLBACK_H
#define K_GAME__ROOM_CALLBACK_H

#include <stdint.h>

#include "k_list.h"

struct k_room;

/* region [room_callback] */

/* 房间回调结点的基础结构
 *
 * 房间的不同类型回调，对应有不同类型的回调结点来存储回调信息。
 * 各类结点具有不同的结构，但都包含此基础结构作为成员字段。
 */
struct k_room_callback {

    /* 标记该回调是否已被删除 */
    unsigned int is_deleted : 1;
};

/* endregion */

/* region [room_callback_list] */

/* 存储房间回调的链表容器
 *
 * 链表这一数据结构足以应对绝大多数场景。
 */
struct k_room_callback_list {
    struct k_list list;
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

/* endregion */

/* region [alarm_callback] */

struct k_room_alarm_callback {

    struct k_list_node list_node;

    struct k_room_callback base;

    uint64_t timeout;

    void (*fn_callback)(void *data, int timeout_diff);

    void *data;
};

struct k_room_alarm_callback_storage {

    /* TODO: 数据结构改用优先队列
     *
     */
    struct k_list list;
};

void k__room_init_alarm_callback_storage(struct k_room *room);

void k__room_cleanup_alarm_callback_storage(struct k_room *room);

void k__room_exec_alarm_callbacks(struct k_room *room);

/* endregion */

/* region [draw_callback] */

struct k_room_draw_callback {

    struct k_list_node list_node;

    struct k_room_callback base;

    void (*fn_callback)(void *data);

    void *data;
};

struct k_room_draw_callback_z_list {

    struct k_list_node list_node;

    struct k_list callbacks;

    int z_index;
};

struct k_room_draw_callback_storage {

    /* TODO: 存储结构改用二叉平衡树
     *
     */
    struct k_list z_lists;
};

void k__room_init_draw_callback_storage(struct k_room *room);

void k__room_cleanup_draw_callback_storage(struct k_room *room);

void k__room_exec_draw_callbacks(struct k_room *room);

/* endregion */

/* region [add_callback] */

struct k_room_callback *k__room_add_enter_callback(struct k_room *room, void (*fn_callback)(void *data), void *data);

struct k_room_callback *k__room_add_leave_callback(struct k_room *room, void (*fn_callback)(void *data), void *data);

struct k_room_callback *k__room_add_step_begin_callback(struct k_room *room, void (*fn_callback)(void *data), void *data);

struct k_room_callback *k__room_add_alarm_callback(struct k_room *room, void (*fn_callback)(void *data, int timeout_diff), void *data, int delay_ms);

struct k_room_callback *k__room_add_step_callback(struct k_room *room, void (*fn_callback)(void *data), void *data);

struct k_room_callback *k__room_add_draw_callback(struct k_room *room, void (*fn_callback)(void *data), void *data, int z_index);

struct k_room_callback *k__room_add_step_end_callback(struct k_room *room, void (*fn_callback)(void *data), void *data);

/* endregion */

#endif
