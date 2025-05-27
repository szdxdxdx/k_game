#ifndef YX_OBJ_PLAYER_H
#define YX_OBJ_PLAYER_H

#include "k_game.h"

#include "config/yx_config_arena_blackboard.h"

#include "object/weapon/yx_obj_weapon_i.h"
#include "utils/state_machine/yx_state_machine.h"

struct yx_obj_player *yx_obj_player_create(void);

/* ------------------------------------------------------------------------ */

struct yx_obj_player {
    struct k_object *object;

    float x;
    float y;

    float hp;
    int ammo;
    int ammo_max;
    float ammo_timer;
    struct k_collision_box *hp_collision_box;

    struct yx_state_machine movement_sm;
    enum k_keyboard_key key_up;
    enum k_keyboard_key key_left;
    enum k_keyboard_key key_down;
    enum k_keyboard_key key_right;
    float vx_movement;
    float vy_movement;
    float speed;

    float vx_knockback;
    float vy_knockback;

    struct k_sprite_renderer *spr_rdr;
    struct k_sprite *spr_idle;
    struct k_sprite *spr_run;

    struct yx_obj_player_weapon *weapon;

    struct yx_config_arena_blackboard *blackboard;

    struct k_position *position;

    struct k_object *object_shadow;
};

int yx__obj_player_on_create_init_movement(struct yx_obj_player *player);

void yx__obj_player_on_step_check_hit_bullet(struct yx_obj_player *player);

int yx__obj_player_on_create_init_collision(struct yx_obj_player *player);

int yx__obj_player_on_create_init_draw(struct yx_obj_player *player);

int yx__obj_player_on_create_add_debug(struct yx_obj_player *player);

#endif
