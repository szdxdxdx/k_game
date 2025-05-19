#ifndef YX_OBJ_RIVAL_H
#define YX_OBJ_RIVAL_H

#include "k_game/core/k_game_fwd.h"

#include "config/yx_config_arena_blackboard.h"

#include "utils/yx_float_vec.h"
#include "utils/state_machine/yx_state_machine.h"

struct yx_obj_rival_config {
    float x;
    float y;
};

struct yx_obj_rival *yx_obj_rival_create(const struct yx_obj_rival_config *config);

/* ------------------------------------------------------------------------ */

enum yx_obj_rival_attack_state {
    YX_OBJ_RIVAL_STATE_PATROL, /* 巡逻 */
    YX_OBJ_RIVAL_STATE_ATTACK, /* 攻击 */
};

struct yx_obj_rival {
    struct k_object *object;

    float x;
    float y;

    int hp;
    struct k_collision_box *hp_collision_box;

    int face; /* 朝向，-1 朝左，1 朝右 */
    struct yx_state_machine move_sm; /* 状态机，负责控制角色的移动和站立 */
    float change_move_state_timer;  /* 计时器，控制切换移动和站立状态 */
    float vx_movement;  /* 移动的速度 */
    float vy_movement;
    float target_position_x; /* 移动的目标位置 */
    float target_position_y;

    float vx_knockback; /* 被子弹击中时，被临时赋予的击退速度 */
    float vy_knockback;

    enum yx_obj_rival_attack_state attack_state;
    struct yx_state_machine attack_sm; /* 状态机，负责控制角色的巡逻和攻击 */

    struct k_sprite_renderer *spr_rdr;
    struct k_sprite *spr_idle;
    struct k_sprite *spr_run;

    struct yx_obj_rival_weapon *weapon;
    float attack_timer;
    float aim_x;
    float aim_y;

    struct yx_config_arena_blackboard *blackboard;

    struct k_position *position;

    /* ------------------------------------------------------------------------ */
};

#define YX__OBJ_RIVAL_AGGRO_RADIUS       500.0f /* 与玩家之间距离小于该值时进入攻击状态 */
#define YX__OBJ_RIVAL_LOSE_TARGET_RADIUS 850.0f /* 与玩家之间距离大于该值时进入巡逻状态 */

int yx__obj_rival_on_create_init_movement(struct yx_obj_rival *rival);

void yx__obj_rival_on_step_check_hit_bullet(struct yx_obj_rival *rival);

int yx__obj_rival_on_create_init_collision(struct yx_obj_rival *rival);

int yx__obj_rival_on_create_init_draw(struct yx_obj_rival *rival);

int yx__obj_rival_on_create_init_debug(struct yx_obj_rival *rival);

int yx__obj_rival_on_create_init_attack(struct yx_obj_rival *rival);

/* ------------------------------------------------------------------------ */

#endif
