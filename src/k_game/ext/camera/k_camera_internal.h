#ifndef K__CAMERA_INTERNAL_H
#define K__CAMERA_INTERNAL_H

#include <stddef.h>

#include "k_game/core/k_object.h"
#include "k_game/core/k_component.h"
#include "k_game/core/k_callback.h"

struct k_camera_target;
struct k_camera;

struct k_camera_target {

    /* 记录 target 在 secondary_targets 中的索引，若是 primary_target 则该值无效 */
    size_t target_idx;

    float *x;
    float *y;

    float weight;
};

int k__camera_target_init(struct k_component *component, void *params);

void k__camera_target_fini(struct k_component *component);

struct k_camera {

    struct k_camera_target *primary_target;

#define K__CAMERA_SECONDARY_TARGET_MAX 24
    struct k_camera_target *secondary_targets[K__CAMERA_SECONDARY_TARGET_MAX];

    size_t secondary_targets_num;

    struct k_callback *cb_camera_step;

    float vx;
    float vy;

    float max_speed;
    float acceleration;
};

extern struct k_component_type *k__camera_component_type;

void k__camera_step(void *camera_);

#endif
