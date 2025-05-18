#ifndef YX_ROOM_ARENA_H
#define YX_ROOM_ARENA_H

#include "k_game/core/k_game_fwd.h"

#include "object/fighter/player/yx_obj_player.h"

struct yx_room_arena {
    struct yx_obj_player *player;
};

struct k_room *yx_room_arena_create(void);

#endif
