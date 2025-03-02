#ifndef K_GAME__ROOM_CALLBACK_ALARM_H
#define K_GAME__ROOM_CALLBACK_ALARM_H

#include <stdint.h>

#include "k_list.h"

#include "./k_room_callback.h"

struct k_room;

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

struct k_room_callback *k__room_add_alarm_callback(struct k_room *room, void (*fn_callback)(void *data, int timeout_diff), void *data, int delay_ms);

#endif
