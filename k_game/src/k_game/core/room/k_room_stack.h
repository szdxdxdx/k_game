#ifndef K__ROOM_STACK_H
#define K__ROOM_STACK_H

struct k_room;

void k__room_stack_init(void);

void k__room_stack_clear(void);

struct k_room *k__room_stack_get_top(void);

#endif
