#include <math.h>
#include <assert.h>

#define K_LOG_TAG "k_game:camera"
#include "k_log.h"

#include "k_game/core/k_view.h"
#include "k_game/core/k_time.h"

#include "./k_camera_internal.h"

void k_camera_shake(float intensity, float duration) {

    struct k_camera *camera = k_room_get_component_manager_data(k__camera_component_type);
    if (NULL == camera) {
        k_log_error("please add a camera to the room first");
        return;
    }

    if (camera->shake_duration < camera->shake_time + duration) {
        camera->shake_duration = camera->shake_time + duration;

        if (camera->shake_decay < intensity) {
            camera->shake_intensity = intensity;
        } else {
            camera->shake_intensity = camera->shake_decay;
        }
    }
    else if (camera->shake_decay < intensity) {
        camera->shake_intensity = intensity;
    }
}

static void k__camera_auto_follow_calc_dst(struct k_camera *camera) {

    if (0 == camera->targets_num) {
        camera->dst_x = camera->curr_x;
        camera->dst_y = camera->curr_y;
        return;
    }

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

        camera->dst_x = sum_wx / sum_w;
        camera->dst_y = sum_wy / sum_w;
    }
    else {
        float sum_wx = 0.0f;
        float sum_wy = 0.0f;
        float sum_w  = 0.0f;

        float view_left;
        float view_top;
        float view_w;
        float view_h;
        k_view_get_rect(&view_left, &view_top, &view_w, &view_h);
        float view_right  = view_left + view_w;
        float view_bottom = view_top + view_h;

        size_t i = 0;
        for (; i < camera->targets_num; i++) {
            struct k_camera_target *target = camera->targets[i];

            float target_x = *target->x;
            float target_y = *target->y;

            if (target_x < view_left)   continue;
            if (target_y < view_top)    continue;
            if (target_x > view_right)  continue;
            if (target_y > view_bottom) continue;

            sum_wx += target_x * target->weight;
            sum_wy += target_y * target->weight;
            sum_w  += target->weight;
        }

        if (0.0f == sum_w) {
            camera->dst_x = *(camera->primary_target->x);
            camera->dst_y = *(camera->primary_target->y);
        }
        else {
            camera->dst_x = sum_wx / sum_w;
            camera->dst_y = sum_wy / sum_w;

            float view_half_w = view_w / 2;
            float view_half_h = view_h / 2;

            float max_cx = *(camera->primary_target->x) + view_half_w;
            if (camera->dst_x > max_cx) {
                camera->dst_x = max_cx;
            } else {
                float min_cx = *(camera->primary_target->x) - view_half_w;
                if (camera->dst_x < min_cx) {
                    camera->dst_x = min_cx;
                }
            }

            float max_cy = *(camera->primary_target->y) + view_half_h;
            if (camera->dst_y > max_cy) {
                camera->dst_y = max_cy;
            } else {
                float min_cy = *(camera->primary_target->y) - view_half_h;
                if (camera->dst_y < min_cy) {
                    camera->dst_y = min_cy;
                }
            }
        }
    }
}

static void k__camera_auto_follow_calc_step(struct k_camera *camera) {

    if (camera->curr_x == camera->dst_x && camera->curr_y == camera->dst_y)
        return;

    float dt = k_time_get_step_delta();

    float dist_x = camera->dst_x - camera->curr_x;
    float dist_y = camera->dst_y - camera->curr_y;
    float dist = sqrtf(dist_x * dist_x + dist_y * dist_y);

    if (dist < 1.0f) {
        camera->vx = 0.0f;
        camera->vy = 0.0f;
        camera->curr_x = camera->dst_x;
        camera->curr_y = camera->dst_y;
    }
    else {
        float dir_x = dist_x / dist;
        float dir_y = dist_y / dist;
        float dot_v = camera->vx * dir_x + camera->vy * dir_y;

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
        camera->curr_x += camera->vx * dt;
        camera->curr_y += camera->vy * dt;
    }
}

static void k__camera_auto_follow_set_view(struct k_camera *camera) {

    if (0.0f == camera->shake_duration) {
        k_view_set_position(camera->curr_x, camera->curr_y);
        return;
    }

    float dt = k_time_get_step_delta();
    camera->shake_time += dt;

    if (camera->shake_duration <= camera->shake_time) {
        camera->shake_intensity = 0.0f;
        camera->shake_decay     = 0.0f;
        camera->shake_duration  = 0.0f;
        camera->shake_time      = 0.0f;
        k_view_set_position(camera->curr_x, camera->curr_y);
    }
    else {
        camera->shake_decay = camera->shake_intensity * (1.0f - camera->shake_time / camera->shake_duration);
        float pi_t = camera->shake_time * 3.1415926f;
        float offset_x = camera->shake_decay * sinf(pi_t * 19.0f);
        float offset_y = camera->shake_decay * cosf(pi_t * 17.0f);
        float view_cx = camera->curr_x + offset_x;
        float view_cy = camera->curr_y + offset_y;
        k_view_set_position(camera->curr_x, camera->curr_y);

        float curr_view_cx;
        float curr_view_cy;
        k_view_get_position(&curr_view_cx, &curr_view_cy);

        if (curr_view_cx != view_cx || curr_view_cy != view_cy) {
            k_view_set_position(curr_view_cx + offset_x, curr_view_cy + offset_y);
        }
    }
}

static void k__camera_auto_follow(struct k_camera *camera) {
    k__camera_auto_follow_calc_dst(camera);
    k__camera_auto_follow_calc_step(camera);
    k__camera_auto_follow_set_view(camera);
}

void k__camera_move_on_begin_step(void *camera_) {
    struct k_camera *camera = camera_;

    switch (camera->state) {
        case K__CAMERA_DISABLE:
            assert(0);
            break;
        case K__CAMERA_AUTO_FOLLOW:
            k__camera_auto_follow(camera);
            break;
        case K__CAMERA_CINEMATIC:
            assert(0);
            break;
    }
}
