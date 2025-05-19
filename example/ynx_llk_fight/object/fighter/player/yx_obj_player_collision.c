
#define K_LOG_TAG "yx:object:player"
#include "k_log.h"

#include "k_game.h"

#include "config/yx_config_collision_group.h"
#include "object/fighter/player/yx_obj_player.h"

static void yx__obj_player_touch_bubble(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);

#if 0
    float x1 = player->x;
    float y1 = player->y;
    float x2 = player->weapon->x;
    float y2 = player->weapon->y;
    struct k_collision_box *box = k_collision_check_rectangle(YX_COLLISION_GROUP_BUBBLE, x1, y1, x2, y2);
    if (NULL != box)
        yx_bubble_pop(k_collision_box_get_object(box));
#endif
}

int yx__obj_player_on_create_add_collision(struct yx_obj_player *player) {

    struct k_collision_rect_config config;
    config.x = &player->x;
    config.y = &player->y;
    config.group_id = YX_CONFIG_COLLISION_GROUP_PLAYER_HP;
    config.offset_x1 = -14.0f;
    config.offset_y1 = -20.0f;
    config.offset_x2 =  14.0f;
    config.offset_y2 =  10.0f;
    player->hp_collision_box = k_object_add_collision_rect(player->object, &config);
    if (NULL == player->hp_collision_box)
        return -1;


    return 0;
}
