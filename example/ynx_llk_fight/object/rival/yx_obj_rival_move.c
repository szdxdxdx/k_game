
#define K_LOG_TAG "yx:object:rival"
#include "k_log.h"

#include "k_game.h"

#include "object/rival/yx_obj_rival.h"

static void yx__obj_rival_on_end_step(struct k_object *object) {
    struct yx_obj_rival *rival = k_object_get_data(object);

    float dt = k_time_get_step_delta();

    float vx = 0.0f;
    float vy = 0.0f;
    {
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
        vx += rival->vx_movement;
        vy += rival->vy_movement;
    }

    rival->x = rival->x + vx * dt;
    rival->y = rival->y + vy * dt;
}

int yx__obj_rival_on_create_add_movement(struct yx_obj_rival *rival) {

    if (NULL == k_camera_add_follow_target(rival->object, &rival->x, &rival->y))
        return -1;

    if (NULL == k_object_add_end_step_callback(rival->object, yx__obj_rival_on_end_step))
        return -1;

    return 0;
}
