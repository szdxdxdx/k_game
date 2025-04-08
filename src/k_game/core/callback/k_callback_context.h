#ifndef K_CALLBACK_CONTEXT_H
#define K_CALLBACK_CONTEXT_H

struct k_room;

/** \brief 删除房间所有的事件回调 */
void k__room_del_all_callbacks(struct k_room *room);

#endif
