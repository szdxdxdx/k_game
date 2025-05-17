#ifndef YX_OBJ_PLAYER_H
#define YX_OBJ_PLAYER_H

#include "k_game.h"

#include "object/weapon/yx_obj_weapon_i.h"

struct yx_obj_player_config {
    float x;
    float y;
    struct k_sprite *spr_idle;
    struct k_sprite *spr_run;
};

struct yx_obj_player *yx_obj_player_create(const struct yx_obj_player_config *config);

/* ------------------------------------------------------------------------ */

enum yx_obj_player_movement_state {
    YX__OBJ_PLAYER_STATE_IDLE,
    YX__OBJ_PLAYER_STATE_RUNNING,
};

struct yx_obj_player {
    struct k_object *object;

    float x;
    float y;

    enum yx_obj_player_movement_state movement_state;
    enum k_keyboard_key key_up;
    enum k_keyboard_key key_left;
    enum k_keyboard_key key_down;
    enum k_keyboard_key key_right;
    float speed;

    struct k_sprite_renderer *spr_rdr;
    struct k_sprite *spr_idle;
    struct k_sprite *spr_run;

    struct yx_obj_weapon_i *weapon;
};

int yx__obj_player_on_create_add_movement(struct yx_obj_player *player);

int yx__obj_player_on_create_add_collision(struct yx_obj_player *player);

int yx__obj_player_on_create_add_draw(struct yx_obj_player *player);

int yx__obj_player_on_create_add_debug(struct yx_obj_player *player);

#endif
