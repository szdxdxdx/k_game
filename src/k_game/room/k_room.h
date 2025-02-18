#ifndef K_GAME__ROOM_H
#define K_GAME__ROOM_H

#include <stdint.h>

#include "k_list.h"
#include "k_str_map.h"

#include "k_game/room.h"

/* region [room_registry] */

struct k_room_registry;

struct k_room_registry_node {

    struct k_list_node iter_node;

    struct k_str_map_node name_map_node;
};

void k__room_registry_init(void);

void k__room_registry_deinit(void);

int k__room_registry_add(struct k_room_registry_node *node, const char *room_name);

void k__room_registry_del(struct k_room_registry_node *node);

/* endregion */

/* region [room_callback] */

/* 房间回调结点的基础结构
 *
 * 房间的不同类型回调，对应有不同类型的回调结点来存储回调信息。
 * 各类结点具有不同的结构，但都包含此基础结构作为成员字段。
 */
struct k_room_callback {

    /* 删除该回调所用的函数
     *
     * k_game 只提供一个方法 `k_room_del_callback()` 来删除房间回调。
     * 由于不同类型的回调结点使用不同的数据结构来存储，对应的删除操作也有所不同。
     * 为了能应对不同情况，每个结点需存储一份用于删除结点自身的函数的指针。
     */
    void (*fn_del_self)(struct k_room_callback *self);
};

/* 存储房间回调的链表容器
 *
 * 链表这一数据结构足以应对绝大多数场景。
 * 房间的 enter、step 等大多数回调都使用链表存储。
 */
struct k_room_callback_list {
    struct k_list list;
};

void k__room_callback_list_init(struct k_room_callback_list *list);

void k__room_callback_list_clean(struct k_room_callback_list *list);

void k__room_callback_list_exec_all(struct k_room_callback_list *list);

struct k_room_draw_callbacks_storage {

    /* TODO: 存储结构改用二叉平衡树
     *
     */
    struct k_list depth_lists;
};

void k__room_init_draw_callbacks_storage(struct k_room *room);

void k__room_del_all_draw_callbacks(struct k_room *room);

void k__room_exec_draw_callbacks(struct k_room *room);

struct k_room_alarm_callbacks_storage {

    /* TODO: 数据结构改用优先队列
     *
     */
    struct k_list list;
};

void k__room_init_alarm_callbacks_storage(struct k_room *room);

void k__room_del_all_alarm_callbacks(struct k_room *room);

void k__room_exec_alarm_callbacks(struct k_room *room);

/* endregion */

/* region [room_context] */

struct k_room {

    int room_w, room_h;

    struct k_room_registry_node room_node;

    int (*fn_create)(struct k_room *room, void *params);

    void (*fn_destroy)(struct k_room *room);

    uint32_t step_interval_ms;

    unsigned int game_loop;

    void *data;

    struct k_room_callback_list enter_callbacks;

    struct k_room_callback_list leave_callbacks;

    struct k_room_callback_list step_begin_callbacks;

    struct k_room_callback_list step_callbacks;

    struct k_room_callback_list step_end_callbacks;

    struct k_room_alarm_callbacks_storage alarm_callbacks;

    struct k_room_draw_callbacks_storage draw_callbacks;
};

void k__room_run(struct k_room *room);

void k__destroy_room(struct k_room *room);

/* endregion */

/* region [room_stack] */

struct k_room_stack;

void k__room_stack_init(void);

void k__room_stack_deinit(void);

/* endregion */

#endif
