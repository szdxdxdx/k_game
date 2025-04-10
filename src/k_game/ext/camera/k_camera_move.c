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

    if (0 == camera->targets_num)
        return;

    float dst_cx;
    float dst_cy;

    if (NULL == camera->primary_target) {
        float sum_wx = 0.0f;
        float sum_wy = 0.0f;
        float sum_w  = 0.0f;

        size_t i = 0;
        for (; i < camera->targets_num; i++) {
            struct k_camera_target *target = camera->targets[i];
            float target_x = *target->x;
            float target_y = *target->y;

            sum_wx += target_x * target->weight;
            sum_wy += target_y * target->weight;
            sum_w  += target->weight;
        }

        dst_cx = sum_wx / sum_w;
        dst_cy = sum_wy / sum_w;
    }
    else {
        float sum_wx = 0.0f;
        float sum_wy = 0.0f;
        float sum_w  = 0.0f;

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
        for (; i < camera->targets_num; i++) {
            struct k_camera_target *target = camera->targets[i];

            float target_x = *target->x;
            float target_y = *target->y;

            if (target_x < curr_view_left)   continue;
            if (target_y < curr_view_top)    continue;
            if (target_x > curr_view_right)  continue;
            if (target_y > curr_view_bottom) continue;

            sum_wx += target_x * target->weight;
            sum_wy += target_y * target->weight;
            sum_w  += target->weight;
        }

        dst_cx = sum_wx / sum_w;
        dst_cy = sum_wy / sum_w;

        float half_view_w = curr_view_w / 2;
        float half_view_h = curr_view_h / 2;

        float max_cx = *(camera->primary_target->x) + half_view_w;
        float min_cx = *(camera->primary_target->x) - half_view_w;
        float max_cy = *(camera->primary_target->y) + half_view_h;
        float min_cy = *(camera->primary_target->y) - half_view_h;

        if (dst_cx < min_cx) {
            dst_cx = min_cx;
        } else if (dst_cx > max_cx) {
            dst_cx = max_cx;
        }

        if (dst_cy < min_cy) {
            dst_cy = min_cy;
        } else if (dst_cy > max_cy) {
            dst_cy = max_cy;
        }
    }

    k__camera_move_towards(camera, dst_cx, dst_cy);
}
