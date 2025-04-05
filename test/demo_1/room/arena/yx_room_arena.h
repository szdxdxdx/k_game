#ifndef YX_ROOM_ARENA_H
#define YX_ROOM_ARENA_H

#include "k_game.h"

struct yx_room_arena {

    struct k_object *player;
};

struct k_room *yx_room_arena_create(void);

void yx_test_bt(void);

#endif
