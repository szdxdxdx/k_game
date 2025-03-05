#ifndef K_GAME__ROOM_CALLBACK_H
#define K_GAME__ROOM_CALLBACK_H

#include <stdint.h>

#include "k_list.h"

// enum k_room_callback_type {
//     K_ROOM_CALLBACK_TYPE_ENTER      = 1,
//     K_ROOM_CALLBACK_TYPE_LEAVE      ,
//     K_ROOM_CALLBACK_TYPE_STEP_BEGIN ,
//     K_ROOM_CALLBACK_TYPE_ALARM      ,
//     K_ROOM_CALLBACK_TYPE_STEP       ,
//     K_ROOM_CALLBACK_TYPE_DRAW       ,
//     K_ROOM_CALLBACK_TYPE_STEP_END   ,
// };

struct k_room;

/* region [room_callback] */

/* 房间回调结点的基础结构
 *
 * 房间的不同类型回调，对应有不同类型的回调结点来存储回调信息。
 * 各类结点具有不同的结构，但都包含此基础结构作为成员字段。
 */
struct k_room_callback {

    /* 标记该回调是否已被删除 */
    uint8_t is_deleted : 1;

    // uint8_t callback_type : 7;
};

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
