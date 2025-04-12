#ifndef YX_ROOM_ARENA_H
#define YX_ROOM_ARENA_H

#include "k_game/core/k_game_fwd.h"

struct yx_room_arena {
    struct k_object *player;
};

struct k_room *yx_create_arena_room(void);

#endif
