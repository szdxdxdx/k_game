#ifndef K_GAME__ROOM_GOTO_H
#define K_GAME__ROOM_GOTO_H

void k_room_stack_init(void);

void k_room_stack_deinit(void);

struct k_room *k_room_stack_get_top(void);

int k_goto_room(struct k_room *room);

#endif
