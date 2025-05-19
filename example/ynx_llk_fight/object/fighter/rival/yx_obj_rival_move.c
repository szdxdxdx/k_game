
#define K_LOG_TAG "yx:object:rival"

#include <assert.h>
#include "k_log.h"

#include "k_game.h"

#include "config/yx_config_arena_blackboard.h"
#include "object/fighter/player/yx_obj_player.h"
#include "object/fighter/rival/yx_obj_rival.h"
#include "object/alert_marker/yx_obj_alert_marker.h"
#include "utils/yx_math.h"

/* region [move] */

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

/* region [idle] */

static void yx__obj_rival_on_state_idle_enter(struct k_object *object) {
    struct yx_obj_rival *rival = k_object_get_data(object);

    switch (rival->attack_state) {
        case YX_OBJ_RIVAL_STATE_PATROL:
            rival->change_move_state_timer = yx_rand(2.0f, 4.0f);
            break;
        case YX_OBJ_RIVAL_STATE_ATTACK:
            rival->change_move_state_timer = yx_rand(1.0f, 2.5f);
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

        float r1 = 160.0f;
        float r2 = 240.0f;
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
}

static void yx__obj_rival_on_step_move(struct k_object *object) {
    struct yx_obj_rival *rival = k_object_get_data(object);

    yx_state_machine_tick(&rival->move_sm);
    yx__obj_rival_on_step_hit_bullet_collision(rival);
    yx__obj_rival_on_step_resolve_movement(rival);

    k_position_set_local_position(rival->position, rival->x, rival->y);
}

/* endregion */

/* region [attack] */

static void yx__obj_rival_on_state_patrol_enter(struct k_object *object);
static void yx__obj_rival_on_state_patrol_update(struct k_object *object);
static struct yx_state_machine_state YX_STATE_PATROL = {
    .on_enter  = yx__obj_rival_on_state_patrol_enter,
    .on_update = yx__obj_rival_on_state_patrol_update,
    .on_leave  = NULL,
};

static void yx__obj_rival_on_state_attack_enter(struct k_object *object);
static void yx__obj_rival_on_state_attack_update(struct k_object *object);
static struct yx_state_machine_state YX_STATE_ATTACK = {
    .on_enter  = yx__obj_rival_on_state_attack_enter,
    .on_update = yx__obj_rival_on_state_attack_update,
    .on_leave  = NULL,
};

/* region [observe_player] */

static int yx__obj_rival_on_step_observe_player(struct yx_obj_rival *rival) {
    struct yx_obj_player *player = rival->blackboard->player;

    if (YX_OBJ_RIVAL_STATE_PATROL == rival->attack_state) {
        if (fabsf(rival->x - player->x) <= 250.0f && fabsf(rival->y - player->y) <= 250.0f) {
            rival->attack_state = YX_OBJ_RIVAL_STATE_ATTACK;
            yx_obj_alert_marker_create(rival->position, -30, -16);
            return 1;
        }
    }

    return 0;
}

/* endregion */

/* region [patrol] */

static void yx__obj_rival_on_state_patrol_enter(struct k_object *object) {

}

static void yx__obj_rival_on_state_patrol_update(struct k_object *object) {

}

/* endregion */

/* region [attack] */

static void yx__obj_rival_on_state_attack_enter(struct k_object *object) {

}

static void yx__obj_rival_on_state_attack_update(struct k_object *object) {

}

/* endregion */

static void yx__obj_rival_on_step_attack(struct k_object *object) {
    struct yx_obj_rival *rival = k_object_get_data(object);


}

/* endregion */

int yx__obj_rival_on_create_add_movement(struct yx_obj_rival *rival) {

    if (NULL == k_camera_add_follow_target(rival->object, &rival->x, &rival->y))
        return -1;
    if (NULL == k_object_add_step_callback(rival->object, yx__obj_rival_on_step_move))
        return -1;
    if (NULL == k_object_add_step_callback(rival->object, yx__obj_rival_on_step_attack))
        return -1;

    yx_state_machine_init(&rival->move_sm, rival->object);
    yx_state_machine_change_state(&rival->move_sm, &YX_STATE_IDLE);

    return 0;
}
