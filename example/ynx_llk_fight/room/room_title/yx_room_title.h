#ifndef YX_ROOM_TITLE_H
#define YX_ROOM_TITLE_H

#include "k_game/core/k_game_fwd.h"

extern struct k_room *yx_room_title;

struct yx_room_title {
    int _;
};

struct k_room *yx_room_title_create(void);

#endif
