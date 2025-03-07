#ifndef K_GAME__ROOM_CREATE_H
#define K_GAME__ROOM_CREATE_H

struct k_room;

void k__room_destroy(struct k_room *room);

/**
 * \brief 销毁房间【此 API 暂未公布】
 *
 * 销毁房间是一件很危险的事，
 * 在游戏运行过程中途销毁房间可能导致不可预期的错误。
 * 建议将房间交由 k_game 在游戏结束时统一销毁。
 */
void k_room_destroy(struct k_room *room);

#endif
