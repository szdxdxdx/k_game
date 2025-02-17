#ifndef K_GAME__ROOM_CALLBACK_ALARM_H
#define K_GAME__ROOM_CALLBACK_ALARM_H

#include "k_list.h"

struct k_room;

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
