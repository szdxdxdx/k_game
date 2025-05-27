
#include <assert.h>

#define K_LOG_TAG "yx:object:rival"
#include "k_log.h"

#include "k_game.h"

#include "config/yx_config_collision_group.h"
#include "config/yx_config_arena_blackboard.h"
#include "object/fighter/player/yx_obj_player.h"
#include "object/fighter/rival/yx_obj_rival.h"
#include "object/particle/yx_obj_alert_marker.h"
#include "object/particle/yx_obj_text_particle.h"
#include "object/particle/yx_obj_particle_on_hit.h"
#include "utils/yx_math.h"

/* region [random_move] */

static void yx__obj_rival_on_state_idle_enter(struct k_object *object);
static void yx__obj_rival_on_state_idle_update(struct k_object *object);
static struct yx_state_machine_state YX_STATE_IDLE = {
    .on_enter  = yx__obj_rival_on_state_idle_enter,
    .on_update = yx__obj_rival_on_state_idle_update,
    .on_leave  = NULL,
};

static void yx__obj_rival_on_state_running_enter(struct k_object *object);
static void yx__obj_rival_on_state_running_update(struct k_object *object);
static struct yx_state_machine_state YX_STATE_RUNNING = {
    .on_enter  = yx__obj_rival_on_state_running_enter,
    .on_update = yx__obj_rival_on_state_running_update,
    .on_leave  = NULL,
};

static void yx__obj_rival_on_state_dead_enter(struct k_object *object);
static void yx__obj_rival_on_state_dead_update(struct k_object *object);
static struct yx_state_machine_state YX_STATE_DEAD = {
    .on_enter  = yx__obj_rival_on_state_dead_enter,
    .on_update = NULL,
    .on_leave  = NULL,
};

/* region [idle] */

static void yx__obj_rival_on_state_idle_enter(struct k_object *object) {
    struct yx_obj_rival *rival = k_object_get_data(object);

    switch (rival->attack_state) {
        case YX_OBJ_RIVAL_STATE_PATROL:
            rival->change_move_state_timer = yx_rand(0.0f, 4.0f);
            break;
        case YX_OBJ_RIVAL_STATE_ATTACK:
            rival->change_move_state_timer = yx_rand(0.0f, 2.5f);
            break;
        default:
            assert(0);
            rival->change_move_state_timer = 1.0f;
            break;
    }

    rival->target_position_x = rival->x;
    rival->target_position_y = rival->y;
    rival->vx_movement = 0.0f;
    rival->vy_movement = 0.0f;
    k_sprite_renderer_set_sprite(rival->spr_rdr, rival->spr_idle);
}

static void yx__obj_rival_on_state_idle_update(struct k_object *object) {
    struct yx_obj_rival *rival = k_object_get_data(object);

    float dt = k_time_get_step_delta();
    rival->change_move_state_timer -= dt;
    if (rival->change_move_state_timer <= 0.0f) {
        rival->change_move_state_timer = 0.0f;

        yx_state_machine_change_state(&rival->move_sm, &YX_STATE_RUNNING);
    }
}

/* endregion */

/* region [running] */

static void yx__obj_rival_random_target_position(struct yx_obj_rival *rival) {

    float target_position_x;
    float target_position_y;

    int max_attempts = 20;

try_again:

    {
        /* 若多次尝试都找不到合适的点，则原地不动 */

        if (--max_attempts == 0) {
            target_position_x = rival->x;
            target_position_y = rival->y;
            goto end;
        }
    }

    {
        /* 函数在人物周围（大于 r1 且 小于 r2 的圆环范围内）取一个随机点 */

        float r1 = 200.0f;
        float r2 = 600.0f;
        float theta = yx_rand(0.0f, 2.0f * 3.1415926f);
        float radius = sqrtf(yx_rand(0.0f, 1.0f) * (r2 * r2 - r1 * r1) + r1 * r1);
        target_position_x = rival->x + radius * cosf(theta);
        target_position_y = rival->y + radius * sinf(theta);
    }
    {
        /* 要求新的目标点必须在房间内 */

        float padding = -30.0f;
        struct k_float_rect room_rect = {
            .x = padding,
            .y = padding,
            .w = k_room_get_w() - padding,
            .h = k_room_get_h() - padding
        };
        if ( ! yx_point_in_rect(target_position_x, target_position_y, &room_rect))
            goto try_again;
    }

    {
        /* 若正在发现了玩家，则不要跑得离玩家太远，且与玩家保持一定距离 */

        if (rival->attack_state == YX_OBJ_RIVAL_STATE_ATTACK) {
            struct yx_obj_player *player = rival->blackboard->player;
            float len = yx_float_vec2_length(yx_float_vec2_new(target_position_x - player->x, target_position_y - player->y));
            if (len < YX__OBJ_RIVAL_AGGRO_RADIUS / 2.0f || YX__OBJ_RIVAL_LOSE_TARGET_RADIUS < len) {
                goto try_again;
            }
        }
    }

end:
    rival->target_position_x = target_position_x;
    rival->target_position_y = target_position_y;
}

static void yx__obj_rival_on_state_running_enter(struct k_object *object) {
    struct yx_obj_rival *rival = k_object_get_data(object);

    yx__obj_rival_random_target_position(rival);

    float speed;
    switch (rival->attack_state) {
        case YX_OBJ_RIVAL_STATE_PATROL:
            speed = yx_rand(140.0f, 160.0f);
            break;
        case YX_OBJ_RIVAL_STATE_ATTACK:
            speed = yx_rand(160.0f, 200.0f);
            break;
        default:
            assert(0);
            speed = 100.0f;
            break;
    }

    struct yx_float_vec2 dir = yx_float_vec2_new(rival->target_position_x - rival->x, rival->target_position_y - rival->y);
    rival->face = dir.x < 0 ? -1 : 1;

    float need_time = yx_float_vec2_length(dir) / speed;
    rival->change_move_state_timer = need_time;

    struct yx_float_vec2 v = yx_float_vec2_scale(yx_float_vec2_normalize(dir), speed);
    rival->vx_movement = v.x;
    rival->vy_movement = v.y;

    k_sprite_renderer_set_sprite(rival->spr_rdr, rival->spr_run);
}

static void yx__obj_rival_on_state_running_update(struct k_object *object) {
    struct yx_obj_rival *rival = k_object_get_data(object);

    int reached_target = 0;
    if (fabsf(rival->x - rival->target_position_x) <= 0.1f && fabsf(rival->y - rival->target_position_y) <= 0.1f) {
        reached_target = 1;
    } else {
        float dt = k_time_get_step_delta();
        rival->change_move_state_timer -= dt;
        if (rival->change_move_state_timer <= 0.0f) {
            reached_target = 1;
        }
    }

    if (reached_target) {
        yx_state_machine_change_state(&rival->move_sm, &YX_STATE_IDLE);
    }
}

/* endregion */

/* region [dead] */

static void yx__obj_rival_on_state_dead_alarm_destroy(struct k_object *object, int timeout_diff) {
    struct yx_obj_rival *rival = k_object_get_data(object);
    yx_obj_rival_destroy(rival);
}

static void yx__obj_rival_on_state_dead_sprite_renderer_callback(struct k_object *object) {
    struct yx_obj_rival *rival = k_object_get_data(object);

    k_object_del_collision_box(rival->hp_collision_box);
    rival->hp_collision_box = NULL;

    k_callback_del(rival->cb_on_step_move);

    // k_object_add_alarm_callback(object, yx__obj_rival_on_state_dead_alarm_destroy, 60000);
}

static void yx__obj_rival_on_state_dead_enter(struct k_object *object) {
    struct yx_obj_rival *rival = k_object_get_data(object);

    k_sprite_renderer_set_sprite(rival->spr_rdr, rival->spr_dead);
    k_sprite_renderer_set_loop_count(rival->spr_rdr, 1);
    k_sprite_renderer_set_loop_callback(rival->spr_rdr, yx__obj_rival_on_state_dead_sprite_renderer_callback);

    yx_state_machine_change_state(&rival->attack_sm, NULL);

   // k_object_del_collision_box(rival->hp_collision_box);
   // rival->hp_collision_box = NULL;

    rival->vx_movement = 0.0f;
    rival->vy_movement = 0.0f;

    yx_obj_rival_weapon_destroy(rival->weapon);
    rival->weapon = NULL;

    k_camera_del_target(rival->camera_target);

    k_callback_del(rival->cb_on_step_attack);
   // k_callback_del(rival->cb_on_step_move);
    k_callback_del(rival->cb_on_step_end_set_face);
    
    struct yx_obj_rival_wave_spawner *spawner = rival->blackboard->rival_wave_spawner;
    spawner->rivals_num -= 1;
    if (0 == spawner->rivals_num) {
        yx_obj_rival_wave_spawner_new_wave(spawner);
    }
}

static void yx__obj_rival_on_state_dead_update(struct k_object *object) {

}

/* endregion */

/* endregion */

/* region [collision] */

static void yx__obj_rival_create_text_particle_on_hit(struct yx_obj_rival *rival, struct yx_bullet_on_hit_result *hit_result) {

    struct yx_float_vec2 v_knockback = {
        .x = hit_result->vx_knockback,
        .y = hit_result->vy_knockback
    };
    struct yx_float_vec2 v_dir_knockback = yx_float_vec2_normalize(v_knockback);
    struct yx_float_vec2 v_text = yx_float_vec2_perp_right(v_dir_knockback);
    if (rand() % 2)
        v_text = yx_float_vec2_neg(v_text);

    v_text = yx_float_vec2_scale(v_text, yx_rand(40.0f, 60.0f));

    struct yx_obj_particle_text_on_hit_config config;
    config.x = rival->x;
    config.y = rival->y;
    config.vx = v_text.x;
    config.vy = v_text.y;
    config.color = 0x660000ff;
    yx_obj_particle_text_on_hit_create(&config, "-%d", (int)hit_result->damage);
}

static void yx__obj_rival_on_step_check_hit_bullet(struct yx_obj_rival *rival) {

    struct k_collision_box *bullet_box = k_collision_check_box(YX_CONFIG_COLLISION_GROUP_PLAYER_BULLET, rival->hp_collision_box);
    if (NULL == bullet_box)
        return;

    struct yx_obj_player_bullet *bullet = k_object_get_data(k_collision_box_get_object(bullet_box));
    struct yx_bullet_on_hit_result hit_result;
    yx_obj_player_bullet_on_hit(bullet, &hit_result);

    yx_obj_particle_on_hit_create(rival->x, rival->y);

    rival->vx_knockback += hit_result.vx_knockback;
    rival->vy_knockback += hit_result.vy_knockback;

    if (&YX_STATE_DEAD != yx_state_machine_get_current_state(&rival->move_sm)) {

        rival->hp -= hit_result.damage;
        yx__obj_rival_create_text_particle_on_hit(rival, &hit_result);

        if (rival->hp <= 0.0f) {
            rival->hp = 0.0f;
            yx_state_machine_change_state(&rival->move_sm, &YX_STATE_DEAD);
        }
    }
}

int yx__obj_rival_on_create_init_collision(struct yx_obj_rival *rival) {

    {
        struct k_collision_rect_config config;
        config.x = &rival->x;
        config.y = &rival->y;
        config.group_id = YX_CONFIG_COLLISION_GROUP_RIVAL_HP;
        config.offset_x1 = -16.0f;
        config.offset_y1 = -24.0f;
        config.offset_x2 = 16.0f;
        config.offset_y2 = 12.0f;
        rival->hp_collision_box = k_object_add_collision_rect(rival->object, &config);
        if (NULL == rival->hp_collision_box)
            return -1;
    }

    return 0;
}

/* endregion */

/* 将所有的速度向量合起来，求解出总速度，然后移动对象 */
static void yx__obj_rival_on_step_resolve_movement(struct yx_obj_rival *rival) {
    float dt = k_time_get_step_delta();

    float vx = 0.0f;
    float vy = 0.0f;

    {
        /* 处理击退 */

        float knockback_damping = 0.85f;
        if (fabsf(rival->vx_knockback) >= 0.01f) {
            vx += rival->vx_knockback;
            rival->vx_knockback *= knockback_damping;
        } else {
            rival->vx_knockback = 0.0f;
        }
        if (fabsf(rival->vy_knockback) >= 0.01f) {
            vy += rival->vy_knockback;
            rival->vy_knockback *= knockback_damping;
        } else {
            rival->vy_knockback = 0.0f;
        }
    }

    {
        /* ai 移动 */

        vx += rival->vx_movement;
        vy += rival->vy_movement;
    }

    rival->x = rival->x + vx * dt; /* 移动对象 */
    rival->y = rival->y + vy * dt;
    k_sprite_renderer_set_z_layer(rival->spr_rdr, (int)rival->y);

    if (NULL != rival->weapon) { /* 移动武器 */
        yx_obj_rival_weapon_set_position(rival->weapon, rival->x, rival->y, (int)rival->y + 1);
    }
}

static void yx__obj_rival_on_step_move(struct k_object *object) {
    struct yx_obj_rival *rival = k_object_get_data(object);

    yx_state_machine_tick(&rival->move_sm);
    yx__obj_rival_on_step_check_hit_bullet(rival);
    yx__obj_rival_on_step_resolve_movement(rival);

    k_position_set_local_position(rival->position, rival->x, rival->y);
}

int yx__obj_rival_on_create_init_movement(struct yx_obj_rival *rival) {

    rival->camera_target = k_camera_add_follow_target(rival->object, &rival->x, &rival->y);
    if (NULL == rival->camera_target) {
        k_log_warn("failed to add camera follow target");
    }

    rival->cb_on_step_move = k_object_add_step_callback(rival->object, yx__obj_rival_on_step_move);
    if (NULL == rival->cb_on_step_move)
        return -1;

    yx_state_machine_init(rival->object, &rival->move_sm);
    yx_state_machine_change_state(&rival->move_sm, &YX_STATE_IDLE);

    return 0;
}
