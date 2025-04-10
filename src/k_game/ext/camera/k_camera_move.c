#include <math.h>

#include "k_game/core/k_view.h"
#include "k_game/core/k_time.h"

#include "./k_camera_internal.h"

static void k__camera_move_towards(struct k_camera *camera, float dst_x, float dst_y) {

    float curr_x;
    float curr_y;
    k_view_get_position(&curr_x, &curr_y);

    if (curr_x == dst_x && curr_y == dst_y)
        return;

    float dist_x = dst_x - curr_x;
    float dist_y = dst_y - curr_y;
    float dist = sqrtf(dist_x * dist_x + dist_y * dist_y);

    float next_x;
    float next_y;

    if (dist < 0.25f) {
        camera->vx = 0.0f;
        camera->vy = 0.0f;
        next_x = dst_x;
        next_y = dst_y;
    }
    else {
        float dir_x = dist_x / dist;
        float dir_y = dist_y / dist;
        float dot_v = camera->vx * dir_x + camera->vy * dir_y;

        float dt = k_get_step_delta();
        float a = camera->acceleration;
        float v;
        float stop_dist = (dot_v * dot_v) / (2.0f * a);
        if (dist <= stop_dist) {
            v = dot_v - a * dt;
            if (v < 0.0f) {
                v = 0.0f;
            }
        } else {
            v = dot_v + a * dt;
            if (v > camera->max_speed) {
                v = camera->max_speed;
            }
        }

        camera->vx = dir_x * v;
        camera->vy = dir_y * v;
        next_x = curr_x + camera->vx * dt;
        next_y = curr_y + camera->vy * dt;
    }

    k_view_set_position(next_x, next_y);
}

void k__camera_step(void *camera_) {
    struct k_camera *camera = camera_;

    struct k_camera_target *primary_target = camera->primary_target;

    if (NULL == primary_target)
        return;

    float dst_cx;
    float dst_cy;

    if (0 == camera->secondary_targets_num) {
        dst_cx = *primary_target->x;
        dst_cy = *primary_target->y;
        goto move_camera;
    }

    float primary_target_x = *primary_target->x;
    float primary_target_y = *primary_target->y;

    float sum_wx = primary_target_x * primary_target->weight;
    float sum_wy = primary_target_y * primary_target->weight;
    float sum_w  = primary_target->weight;

    float curr_view_x;
    float curr_view_y;
    float curr_view_w;
    float curr_view_h;
    k_view_get_rect(&curr_view_x, &curr_view_y, &curr_view_w, &curr_view_h);

    float curr_view_left   = curr_view_x;
    float curr_view_top    = curr_view_y;
    float curr_view_right  = curr_view_x + curr_view_w;
    float curr_view_bottom = curr_view_y + curr_view_h;

    size_t i = 0;
    for (; i < camera->secondary_targets_num; i++) {
        struct k_camera_target *secondary_target = camera->secondary_targets[i];

        float secondary_target_x = *secondary_target->x;
        float secondary_target_y = *secondary_target->y;

        if (secondary_target_x < curr_view_left)   continue;
        if (secondary_target_y < curr_view_top)    continue;
        if (secondary_target_x > curr_view_right)  continue;
        if (secondary_target_y > curr_view_bottom) continue;

        sum_wx += secondary_target_x * secondary_target->weight;
        sum_wy += secondary_target_y * secondary_target->weight;
        sum_w  += secondary_target->weight;
    }

    dst_cx = sum_wx / sum_w;
    dst_cy = sum_wy / sum_w;

    float half_view_w = curr_view_w / 2;
    float max_cx = primary_target_x + half_view_w;
    float min_cx = primary_target_x - half_view_w;
    if (dst_cx < min_cx) {
        dst_cx = min_cx;
    }
    else if (dst_cx > max_cx) {
        dst_cx = max_cx;
    }

    float half_view_h = curr_view_h / 2;
    float max_cy = primary_target_y + half_view_h;
    float min_cy = primary_target_y - half_view_h;
    if (dst_cy < min_cy) {
        dst_cy = min_cy;
    } else if (dst_cy > max_cy) {
        dst_cy = max_cy;
    }

move_camera:
    k__camera_move_towards(camera, dst_cx, dst_cy);
}
