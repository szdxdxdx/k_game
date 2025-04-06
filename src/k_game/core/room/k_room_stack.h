#ifndef K_ROOM_STACK_H
#define K_ROOM_STACK_H

struct k_room;

void k__init_room_stack(void);

void k__cleanup_room_stack(void);

struct k_room *k__room_stack_get_top(void);

#endif
