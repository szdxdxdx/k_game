#ifndef YX_ROOM_ARENA_H
#define YX_ROOM_ARENA_H

#include "k_game/core/k_room.h"

struct yx_room_arena {
    struct k_object *player;
};

struct k_room *yx_room_arena_create(void);

#endif
