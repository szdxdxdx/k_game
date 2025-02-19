#ifndef K_GAME__ROOM_GOTO_H
#define K_GAME__ROOM_GOTO_H

#include <stddef.h>

struct k_room;

#ifndef K_ROOM_STACK_MAX_SIZE
#define K_ROOM_STACK_MAX_SIZE 32
#endif

struct k_room_stack {

    struct k_room *rooms[K_ROOM_STACK_MAX_SIZE];

    size_t top;
};

void k__room_stack_init(void);

void k__room_stack_deinit(void);

struct k_room *k__room_stack_get_top(void);

#endif
