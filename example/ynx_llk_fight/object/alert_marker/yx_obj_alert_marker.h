
#ifndef YX_OBJ_ALERT_MARKER_H
#define YX_OBJ_ALERT_MARKER_H

#include "k_game.h"

struct yx_obj_alert_marker {
    float x;
    float y;
};

struct yx_obj_alert_marker *yx_obj_alert_marker_create(struct k_position *position, float offset_x, float offset_y);

#endif
