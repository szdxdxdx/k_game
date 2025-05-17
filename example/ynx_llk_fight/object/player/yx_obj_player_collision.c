
#define K_LOG_TAG "yx:object:player"
#include "k_log.h"

#include "k_game.h"

#include "object/player/yx_obj_player.h"

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
    return 0;
}
