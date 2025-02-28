#ifndef K_GAME__ROOM_CALLBACK_H
#define K_GAME__ROOM_CALLBACK_H

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

#endif
