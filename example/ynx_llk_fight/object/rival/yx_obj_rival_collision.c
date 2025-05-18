
#define K_LOG_TAG "yx:object:rival"
#include "k_log.h"

#include "k_game.h"

#include "config/yx_config_collision_group.h"
#include "object/rival/yx_obj_rival.h"

int yx__obj_rival_on_create_add_collision(struct yx_obj_rival *rival) {

    struct k_collision_rect_config config;
    config.x = &rival->x;
    config.y = &rival->y;
    config.group_id = YX_CONFIG_COLLISION_GROUP_FIGHTER;
    config.offset_x1 = -16.0f;
    config.offset_y1 = -24.0f;
    config.offset_x2 =  16.0f;
    config.offset_y2 =  12.0f;
    rival->hp_collision_box = k_object_add_collision_rect(rival->object, &config);
    if (NULL == rival->hp_collision_box)
        return -1;

    return 0;
}
