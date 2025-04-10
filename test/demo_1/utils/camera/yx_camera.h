#ifndef YX_CAMERA_H
#define YX_CAMERA_H

#include "k_game.h"

struct yx_camera_target;

int yx_camera_component_define(void);

int yx_room_add_camera(void);

struct yx_camera_target *yx_object_add_camera_follow(struct k_object *object, float *x, float *y);

int yx_camera_set_primary_target(struct yx_camera_target *target);

int yx_camera_target_set_weight(struct yx_camera_target *target, float weight);

#endif
