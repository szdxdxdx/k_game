#include <math.h>

#include "./yx_camera.h"

/* region [struct_def] */

struct yx_camera_target;
struct yx_camera_manager;

struct yx_camera_target {

    struct yx_camera_manager *manager;

    /* 记录 target 在 secondary_targets 中的索引，若是 main_target 则该值无效 */
    size_t target_idx;

    float *x;
    float *y;

    float weight;
};

struct yx_camera_manager {

    struct yx_camera_target *main_target;

#define YX__CAMERA_SECONDARY_TARGET_MAX 16
    struct yx_camera_target *secondary_targets[YX__CAMERA_SECONDARY_TARGET_MAX];

    size_t secondary_targets_num;

    struct k_callback *cb_camera_move;
};

static struct k_component_type *yx__camera_component_type;

/* endregion */

/* region [camera_move] */

void yx_camera_move(void *camera_manager) {
    struct yx_camera_manager *manager = camera_manager;

    struct yx_camera_target *main_target = manager->main_target;
    if (NULL == main_target)
        return;

    float current_view_x;
    float current_view_y;
    float current_view_w;
    float current_view_h;
    k_view_get_rect(&current_view_x, &current_view_y, &current_view_w, &current_view_h);

    float sum_wx = (*main_target->x) * main_target->weight;
    float sum_wy = (*main_target->y) * main_target->weight;
    float sum_w = main_target->weight;

    float min_x = current_view_x;
    float min_y = current_view_y;
    float max_x = current_view_x + current_view_w;
    float max_y = current_view_y + current_view_h;

    struct yx_camera_target *secondary_target;
    size_t i = 0;
    for (; i < manager->secondary_targets_num; i++) {
        secondary_target = manager->secondary_targets[i];

        if ((*secondary_target->x) < min_x) { continue; }
        if ((*secondary_target->x) > max_x) { continue; }
        if ((*secondary_target->y) < min_y) { continue; }
        if ((*secondary_target->y) > max_y) { continue; }

        sum_wx += (*secondary_target->x) * secondary_target->weight;
        sum_wy += (*secondary_target->y) * secondary_target->weight;
        sum_w  += secondary_target->weight;
    }

    float dst_x = sum_wx / sum_w;
    float dst_y = sum_wy / sum_w;

    float cx;
    float cy;
    k_view_get_position(&cx, &cy);

    float new_cx;
    float new_cy;
    float dx = dst_x - cx;
    float dy = dst_y - cy;
    if (dx * dx + dy * dy <= 0.4f) {
        new_cx = dst_x;
        new_cy = dst_y;
    } else {
        float delta = k_get_step_delta();
        float factor = 2.0f - 2.0f * expf(-delta);
        new_cx = cx + dx * factor;
        new_cy = cy + dy * factor;
    }
    k_view_set_position(new_cx, new_cy);
}

/* endregion */

/* region [camera_set] */

int yx_camera_set_main_target(struct yx_camera_target *target) {

    if (NULL == target)
        return -1;

    struct yx_camera_manager *manager = k_room_get_component_manager_data(yx__camera_component_type);
    if (NULL == manager)
        return -1;

    if (manager->main_target == target)
        return 0;

    if (NULL == manager->main_target) {

        size_t last_idx = manager->secondary_targets_num - 1;
        if (target->target_idx != last_idx) {
            struct yx_camera_target *swap_target = manager->secondary_targets[last_idx];
            swap_target->target_idx = target->target_idx;
            manager->secondary_targets[target->target_idx] = swap_target;
        }
        manager->secondary_targets_num -= 1;
    }
    else {
        struct yx_camera_target *old_main_target = manager->main_target;
        old_main_target->target_idx = target->target_idx;
        manager->secondary_targets[target->target_idx] = old_main_target;
    }

    manager->main_target = target;
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

    struct yx_camera_manager *manager = k_component_get_manager_data(component);
    if (NULL == manager)
        return -1;

    if (YX__CAMERA_SECONDARY_TARGET_MAX <= manager->secondary_targets_num)
        return -1;

    float **xy = (float **)params;

    struct yx_camera_target *target = k_component_get_data(component);
    target->manager = manager;
    target->x = xy[0];
    target->y = xy[1];
    target->weight = 1.0f;

    size_t top_idx = manager->secondary_targets_num;
    target->target_idx = top_idx;
    manager->secondary_targets[top_idx] = target;
    manager->secondary_targets_num += 1;

    return 0;
}

void yx_camera_target_fini(struct k_component *component) {
    struct yx_camera_target *target_to_del = k_component_get_data(component);
    struct yx_camera_manager *manager = k_component_get_manager_data(component);

    if (target_to_del == manager->main_target) {
        manager->main_target = NULL;
    }
    else {
        size_t last_idx = manager->secondary_targets_num - 1;
        if (target_to_del->target_idx != last_idx) {
            struct yx_camera_target *swap_target = manager->secondary_targets[last_idx];
            swap_target->target_idx = target_to_del->target_idx;
            manager->secondary_targets[target_to_del->target_idx] = swap_target;
        }
        manager->secondary_targets_num -= 1;
    }
}

/* endregion */

/* region [camera_manager] */

int yx_camera_manager_init(struct k_component_manager *component_manager, void *params) {
    (void)params;

    struct yx_camera_manager *manager = k_component_manager_get_data(component_manager);

    manager->cb_camera_move = k_room_add_step_end_callback(manager, yx_camera_move);
    if (NULL == manager->cb_camera_move)
        return -1;

    manager->main_target = NULL;
    manager->secondary_targets_num = 0;

    return 0;
}

void yx_camera_manager_fini(struct k_component_manager *component_manager) {
    struct yx_camera_manager *manager = k_component_manager_get_data(component_manager);

    k_room_del_callback(manager->cb_camera_move);
}

/* endregion */

/* region [camera_typedef] */

int yx_camera_component_define(void) {

    struct k_component_entity_config entity_config = K_COMPONENT_ENTITY_CONFIG_INIT;
    entity_config.data_size = sizeof(struct yx_camera_target);
    entity_config.fn_init = yx_camera_target_init;
    entity_config.fn_fini = yx_camera_target_fini;

    struct k_component_manager_config manager_config = K_COMPONENT_MANAGER_CONFIG_INIT;
    manager_config.data_size = sizeof(struct yx_camera_manager);
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
