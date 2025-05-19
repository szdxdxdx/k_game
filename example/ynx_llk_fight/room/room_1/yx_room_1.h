#ifndef YX_ROOM_1_H
#define YX_ROOM_1_H

#include "k_game/core/k_game_fwd.h"

struct yx_room_1 {
    struct yx_obj_player *player;
    int draw_background;
};

struct k_room *yx_room_1_create(void);

int yx__room_1_on_create_set_bkgd(void);

int yx__room_1_on_create_place_obj(void);

int yx__room_1_on_create_init_webui(void);

extern struct k_room *yx_room_1;

#endif
