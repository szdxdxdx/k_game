#ifndef YX_OBJ_RIVAL_H
#define YX_OBJ_RIVAL_H

#include "k_game/core/k_game_fwd.h"
#include "utils/yx_float_vec.h"

struct yx_obj_rival_config {
    float x;
    float y;
};

struct yx_obj_rival *yx_obj_rival_create(const struct yx_obj_rival_config *config);

/* ------------------------------------------------------------------------ */

enum yx_enemy_ai_state {
    YX_ENEMY_AI_PATROL, /* 正在巡逻，没有发现玩家 */
    YX_ENEMY_AI_ATTACK, /* 发现了玩家，靠近或攻击 */
};

enum yx_enemy_move_state {
    YX_ENEMY_MOVE_IDLE,    /* 静止 */
    YX_ENEMY_MOVE_RUNNING, /* 正在移动 */
};

struct yx_obj_rival {
    struct k_object *object;

    float x;
    float y;

    struct k_collision_box *hp_collision_box;

    float vx_movement;
    float vy_movement;

    float vx_knockback;
    float vy_knockback;

    uint8_t ai_state;
    uint8_t move_state;

    struct k_sprite_renderer *spr_rdr;
    struct k_sprite *spr_idle;
    struct k_sprite *spr_run;

    struct yx_obj_weapon_i *weapon;

    struct yx_config_arena_blackboard *blackboard;
};

int yx__obj_rival_on_create_add_movement(struct yx_obj_rival *rival);

void yx__obj_rival_on_step_hit_bullet_collision(struct yx_obj_rival *rival);

int yx__obj_rival_on_create_add_collision(struct yx_obj_rival *rival);

int yx__obj_rival_on_create_add_draw(struct yx_obj_rival *rival);

#endif
