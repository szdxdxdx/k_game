#include <math.h>

#include "./yx_camera.h"

/* region [struct_def] */

struct yx_camera_target;
struct yx_camera;

struct yx_camera_target {

    /* 记录 target 在 secondary_targets 中的索引，若是 primary_target 则该值无效 */
    size_t target_idx;

    float *x;
    float *y;

    float weight;
};

struct yx_camera {

    struct yx_camera_target *primary_target;

#define YX__CAMERA_SECONDARY_TARGET_MAX 16
    struct yx_camera_target *secondary_targets[YX__CAMERA_SECONDARY_TARGET_MAX];

    size_t secondary_targets_num;

    struct k_callback *cb_camera_step;

    float vx;
    float vy;
    float max_v;
    float a;
};

static struct k_component_type *yx__camera_component_type;

/* endregion */

/* region [camera_move] */

void yx_camera_move_towards(struct yx_camera *camera, float dst_x, float dst_y) {

    float curr_x;
    float curr_y;
    k_view_get_position(&curr_x, &curr_y);

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
        float v;
        if (dist <= (dot_v * dot_v) / (2.0f * camera->a)) {
            v = dot_v - camera->a * dt;
            if (v < 0.0f) {
                v = 0.0f;
            }
        } else {
            v = dot_v + camera->a * dt;
            if (v > camera->max_v) {
                v = camera->max_v;
            }
        }

        camera->vx = dir_x * v;
        camera->vy = dir_y * v;
        next_x = curr_x + camera->vx * dt;
        next_y = curr_y + camera->vy * dt;
    }

    k_view_set_position(next_x, next_y);
}

void yx_camera_step(void *camera_) {
    struct yx_camera *camera = camera_;

    struct yx_camera_target *primary_target = camera->primary_target;

    if (NULL == primary_target)
        return;

    float curr_view_x;
    float curr_view_y;
    float curr_view_w;
    float curr_view_h;
    k_view_get_rect(&curr_view_x, &curr_view_y, &curr_view_w, &curr_view_h);

    float sum_wx = (*primary_target->x) * primary_target->weight;
    float sum_wy = (*primary_target->y) * primary_target->weight;
    float sum_w = primary_target->weight;

    size_t i = 0;
    for (; i < camera->secondary_targets_num; i++) {
        struct yx_camera_target *secondary_target = camera->secondary_targets[i];

        if ((*secondary_target->x) < curr_view_x) continue;
        if ((*secondary_target->y) < curr_view_y) continue;
        if ((*secondary_target->x) > curr_view_x + curr_view_w) continue;
        if ((*secondary_target->y) > curr_view_y + curr_view_h) continue;

        sum_wx += (*secondary_target->x) * secondary_target->weight;
        sum_wy += (*secondary_target->y) * secondary_target->weight;
        sum_w  += secondary_target->weight;
    }

    float dst_cx = sum_wx / sum_w;
    float dst_cy = sum_wy / sum_w;



    yx_camera_move_towards(camera, dst_cx, dst_cy);
}

/* endregion */

/* region [camera_set] */

int yx_camera_set_primary_target(struct yx_camera_target *target) {

    if (NULL == target)
        return -1;

    struct yx_camera *camera = k_room_get_component_manager_data(yx__camera_component_type);
    if (NULL == camera)
        return -1;

    if (camera->primary_target == target)
        return 0;

    if (NULL == camera->primary_target) {

        size_t last_idx = camera->secondary_targets_num - 1;
        if (target->target_idx != last_idx) {
            struct yx_camera_target *swap_target = camera->secondary_targets[last_idx];
            swap_target->target_idx = target->target_idx;
            camera->secondary_targets[target->target_idx] = swap_target;
        }
        camera->secondary_targets_num -= 1;
    }
    else {
        struct yx_camera_target *old_main_target = camera->primary_target;
        old_main_target->target_idx = target->target_idx;
        camera->secondary_targets[target->target_idx] = old_main_target;
    }

    camera->primary_target = target;
    return 0;
}

int yx_camera_target_set_weight(struct yx_camera_target *target, float weight) {

    if (NULL == target && weight <= 0.0f) {
        return -1;
    } else {
        target->weight = weight;
        return 0;
    }
}

/* endregion */

/* region [camera_target] */

int yx_camera_target_init(struct k_component *component, void *params) {

    struct yx_camera *camera = k_component_get_manager_data(component);
    if (NULL == camera)
        return -1;

    if (YX__CAMERA_SECONDARY_TARGET_MAX <= camera->secondary_targets_num)
        return -1;

    float **xy = (float **)params;

    struct yx_camera_target *target = k_component_get_data(component);
    target->x = xy[0];
    target->y = xy[1];
    target->weight = 1.0f;

    size_t top_idx = camera->secondary_targets_num;
    target->target_idx = top_idx;
    camera->secondary_targets[top_idx] = target;
    camera->secondary_targets_num += 1;

    return 0;
}

void yx_camera_target_fini(struct k_component *component) {
    struct yx_camera_target *target_to_del = k_component_get_data(component);
    struct yx_camera *camera = k_component_get_manager_data(component);

    if (target_to_del == camera->primary_target) {
        camera->primary_target = NULL;
    }
    else {
        size_t last_idx = camera->secondary_targets_num - 1;
        if (target_to_del->target_idx != last_idx) {
            struct yx_camera_target *swap_target = camera->secondary_targets[last_idx];
            swap_target->target_idx = target_to_del->target_idx;
            camera->secondary_targets[target_to_del->target_idx] = swap_target;
        }
        camera->secondary_targets_num -= 1;
    }
}

/* endregion */

/* region [camera_manager] */

int yx_camera_manager_init(struct k_component_manager *component_manager, void *params) {
    (void)params;

    struct yx_camera *camera = k_component_manager_get_data(component_manager);

    camera->cb_camera_step = k_room_add_step_end_callback(camera, yx_camera_step);
    if (NULL == camera->cb_camera_step)
        return -1;

    camera->primary_target = NULL;
    camera->secondary_targets_num = 0;

    camera->vx = 0.0f;
    camera->vy = 0.0f;
    camera->max_v = 200.0f;
    camera->a = 200.0f;

    return 0;
}

void yx_camera_manager_fini(struct k_component_manager *component_manager) {
    struct yx_camera *camera = k_component_manager_get_data(component_manager);

    k_room_del_callback(camera->cb_camera_step);
}

/* endregion */

/* region [camera_typedef] */

int yx_camera_component_define(void) {

    struct k_component_entity_config entity_config = K_COMPONENT_ENTITY_CONFIG_INIT;
    entity_config.data_size = sizeof(struct yx_camera_target);
    entity_config.fn_init = yx_camera_target_init;
    entity_config.fn_fini = yx_camera_target_fini;

    struct k_component_manager_config manager_config = K_COMPONENT_MANAGER_CONFIG_INIT;
    manager_config.data_size = sizeof(struct yx_camera);
    manager_config.fn_init = yx_camera_manager_init;
    manager_config.fn_fini = yx_camera_manager_fini;

    struct k_component_type *type = k_component_define(&manager_config, &entity_config);
    if (NULL == type)
        return -1;

    yx__camera_component_type = type;
    return 0;
}

int yx_room_add_camera(void) {
    return k_room_add_component_manager(yx__camera_component_type, NULL);
}

struct yx_camera_target *yx_object_add_camera_follow(struct k_object *object, float *x, float *y) {

    if (NULL == x || NULL == y)
        return NULL;

    float *xy[2] = { x, y };

    struct k_component *component = k_object_add_component(object, yx__camera_component_type, xy);
    if (NULL == component)
        return NULL;

    return k_component_get_data(component);
}

/* endregion */
