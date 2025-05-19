
#ifndef YX_OBJ_ALERT_MARKER_H
#define YX_OBJ_ALERT_MARKER_H

#include "k_game.h"

struct yx_obj_alert_marker {
    struct k_position *position;
};

void yx_obj_alert_marker_create(struct k_position *position, float x, float y);

#endif
