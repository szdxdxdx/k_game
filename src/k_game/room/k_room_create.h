#ifndef K_GAME__ROOM_CREATE_H
#define K_GAME__ROOM_CREATE_H

struct k_room;

/* region [room_registry] */

int k__room_registry_init(void);

void k__room_registry_cleanup(void);

/* endregion */

/**
 * \brief 销毁房间【暂未公布的 API】
 *
 * 销毁房间是一件很危险的事，
 * 在游戏运行过程中途销毁房间可能导致不可预期的错误。
 *
 * 房间牵扯的资源太多了。销毁单个房间时，释放资源很麻烦，
 * 而销毁所有房间时，释放资源就好办多了。
 *
 * 我建议让 k_game 在游戏结束时统一销毁所有房间。
 */
void k_room_destroy(struct k_room *room);

#endif
