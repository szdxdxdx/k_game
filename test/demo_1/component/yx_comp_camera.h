#ifndef YX_COMP_CAMERA_H
#define YX_COMP_CAMERA_H

#include "k_game.h"

struct yx_camera_target;

int yx_camera_component_define(void);

int yx_room_add_camera(void);

struct yx_camera_target *yx_object_add_camera_follow(struct k_object *object, float *x, float *y);

void yx_camera_set_main_target(struct yx_camera_target *target);

void yx_camera_target_set_weight(struct yx_camera_target *target, float weight);

#endif
