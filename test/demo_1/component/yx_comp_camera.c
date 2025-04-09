#include <math.h>

#include "./yx_comp_camera.h"

struct yx_camera_target;
struct yx_camera_manager;

struct yx_camera_target {

    struct yx_camera_manager *manager;

    size_t target_idx;

    float *x;
    float *y;
};

struct yx_camera_manager {

#define YX__CAMERA_TARGET_MAX 16
    struct yx_camera_target *targets[YX__CAMERA_TARGET_MAX];

    size_t targets_num;

    struct k_callback *cb_camera_move;
};

void yx_camera_move(void *camera_manager) {
    struct yx_camera_manager *manager = camera_manager;

    if (0 == manager->targets_num)
        return;

    struct yx_camera_target *target = manager->targets[0];
    float min_x = *target->x;
    float max_x = *target->x;
    float min_y = *target->y;
    float max_y = *target->y;

    size_t i = 1;
    for (; i < manager->targets_num; i++) {
        target = manager->targets[i];
        if (*target->x < min_x) { min_x = *target->x; }
        if (*target->x > max_x) { max_x = *target->x; }
        if (*target->y < min_y) { min_y = *target->y; }
        if (*target->y > max_y) { max_y = *target->y; }
    }

    float cx = (min_x + max_x) * 0.5f;
    float cy = (min_y + max_y) * 0.5f;

    float w = fmaxf(150.0f, max_x - min_x + 200.0f);
    float h = fmaxf(100.0f, max_y - min_y + 200.0f);
    k_view_fit_rect(w, h);

    k_view_set_position(cx, cy);
}

int yx_camera_target_init(struct k_component *component, void *params) {

    struct yx_camera_manager *manager = k_component_get_manager_data(component);

    if (YX__CAMERA_TARGET_MAX <= manager->targets_num)
        return -1;

    struct yx_camera_target *target = k_component_get_data(component);

    target->manager = manager;
    target->target_idx = manager->targets_num;

    manager->targets[manager->targets_num] = target;
    manager->targets_num += 1;

    float **xy = (float **)params;
    target->x = xy[0];
    target->y = xy[1];

    return 0;
}

void yx_camera_target_fini(struct k_component *component) {
    struct yx_camera_target *target = k_component_get_data(component);
    struct yx_camera_manager *manager = k_component_get_manager_data(component);

    struct yx_camera_target *last_target = manager->targets[manager->targets_num - 1];
    if (target != last_target) {
        last_target->target_idx = target->target_idx;
        manager->targets[target->target_idx] = last_target;
    }
    manager->targets_num -= 1;
}

int yx_camera_manager_init(struct k_component_manager *component_manager, void *params) {
    (void *)params;

    struct yx_camera_manager *manager = k_component_manager_get_data(component_manager);
    manager->targets_num = 0;

    manager->cb_camera_move = k_room_add_step_end_callback(manager, yx_camera_move);
    if (NULL == manager->cb_camera_move)
        return -1;

    return 0;
}

void yx_camera_manager_fini(struct k_component_manager *component_manager) {
    struct yx_camera_manager *manager = k_component_manager_get_data(component_manager);

    k_room_del_callback(manager->cb_camera_move);
}

static struct k_component_type *yx__camera_component_type;

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

void yx_room_add_camera_manager(void) {
    k_room_add_component_manager(yx__camera_component_type, NULL);
}

int yx_object_add_camera_follow(struct k_object *object, float *x, float *y) {

    if (NULL == object || NULL == x || NULL == y)
        return -1;

    float *xy[2] = { x, y };

    struct k_component *component = k_object_add_component(object, yx__camera_component_type, xy);
    if (NULL == component)
        return -1;
    else
        return 0;
}
