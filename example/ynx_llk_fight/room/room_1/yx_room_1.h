#ifndef YX_ROOM_1_H
#define YX_ROOM_1_H

#include "k_game/core/k_game_fwd.h"

struct yx_room_1 {
    struct yx_config_arena_blackboard *arena_blackboard;

    int draw_background;
};

extern struct k_room *yx_room_1;

struct k_room *yx_room_1_create(void);

int yx__room_1_init_ui(void);

int yx__room_1_on_create_set_bkgd(void);

int yx__room_1_on_enter_init_webui(void);

#endif
