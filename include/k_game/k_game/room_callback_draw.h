#ifndef K_GAME__ROOM_CALLBACK_DRAW_H
#define K_GAME__ROOM_CALLBACK_DRAW_H

#include "k_list.h"

#include "k_game/room_callback.h"

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

#endif
