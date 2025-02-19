#ifndef K_GAME__ROOM_CALLBACK_ALARM_H
#define K_GAME__ROOM_CALLBACK_ALARM_H

#include <stdint.h>

#include "k_list.h"

#include "./k_room_callback.h"

struct k_room;

struct k_room_alarm_callback {

    struct k_room_callback impl;

    struct k_list_node list_node;

    void *data;

    void (*fn_callback)(void *data, int timeout_diff);

    uint64_t timeout;
};

struct k_room_alarm_callbacks_storage {

    /* TODO: 数据结构改用优先队列
     *
     */
    struct k_list list;
};

void k__room_init_alarm_callbacks_storage(struct k_room *room);

void k__room_del_all_alarm_callbacks(struct k_room *room);

void k__room_exec_alarm_callbacks(struct k_room *room);

#endif
