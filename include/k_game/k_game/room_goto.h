#ifndef K_GAME__ROOM_GOTO_H
#define K_GAME__ROOM_GOTO_H

#include <stddef.h>

struct k_room;

void k__room_stack_init(void);

void k__room_stack_cleanup(void);

struct k_room *k__room_stack_get_top(void);

#endif
