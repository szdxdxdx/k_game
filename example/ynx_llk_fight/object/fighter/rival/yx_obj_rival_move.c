
#define K_LOG_TAG "yx:object:rival"
#include "k_log.h"

#include "k_game.h"

#include "config/yx_config_arena_blackboard.h"
#include "object/fighter/player/yx_obj_player.h"
#include "object/fighter/rival/yx_obj_rival.h"

static void yx__obj_rival_on_step_ai_movement(struct yx_obj_rival *rival) {

    struct yx_obj_player *player = rival->blackboard->player;
    if (NULL == player)
        return;

    struct yx_float_vec2 dir_to_player;
    dir_to_player.x = player->x - rival->x;
    dir_to_player.y = player->y - rival->y;

    dir_to_player = yx_float_vec2_normalize(dir_to_player);

    float speed = 40.0f;
    float dt = k_time_get_step_delta();
    struct yx_float_vec2 v_movement = yx_float_vec2_scale(dir_to_player, speed);

    rival->vx_movement = v_movement.x;
    rival->vy_movement = v_movement.y;
}

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

static void yx__obj_rival_on_step(struct k_object *object) {
    struct yx_obj_rival *rival = k_object_get_data(object);

    yx__obj_rival_on_step_ai_movement(rival);
    yx__obj_rival_on_step_hit_bullet_collision(rival);
    yx__obj_rival_on_step_resolve_movement(rival);
}

int yx__obj_rival_on_create_add_movement(struct yx_obj_rival *rival) {

    if (NULL == k_camera_add_follow_target(rival->object, &rival->x, &rival->y))
        return -1;

    if (NULL == k_object_add_step_callback(rival->object, yx__obj_rival_on_step))
        return -1;

    return 0;
}
