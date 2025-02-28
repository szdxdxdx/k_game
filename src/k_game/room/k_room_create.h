#ifndef K_GAME__ROOM_CREATE_H
#define K_GAME__ROOM_CREATE_H

struct k_room;

/**
 * \brief 删除房间
 *
 * 【 此 API 暂未公布 】删除房间是一件很危险的事。
 *
 * 在游戏运行过程中途删除房间可能导致不可预期的错误，
 * 建议将房间交由 k_game 在游戏结束时统一删除。
 */
void k_destroy_room(struct k_room *room);

/* 删除房间
 *
 */
void k__destroy_room(struct k_room *room);

#endif
