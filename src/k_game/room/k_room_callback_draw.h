#ifndef K_GAME__ROOM_CALLBACK_DRAW_H
#define K_GAME__ROOM_CALLBACK_DRAW_H

#include "k_list.h"

#include "k_room_callback.h"

struct k_room;

struct k_room_draw_callback {

    struct k_room_callback impl;

    struct k_room_draw_callback_depth_list *depth_list;

    struct k_list_node list_node;

    void *data;

    void (*fn_callback)(void *data);
};

struct k_room_draw_callback_depth_list {

    struct k_list_node list_node;

    struct k_list callbacks_list;

    int depth;
};

struct k_room_draw_callbacks_storage {

    /* TODO: 存储结构改用二叉平衡树
     *
     */
    struct k_list depth_lists;
};

void k__room_init_draw_callbacks_storage(struct k_room *room);

void k__room_del_all_draw_callbacks(struct k_room *room);

void k__room_exec_draw_callbacks(struct k_room *room);

#endif
