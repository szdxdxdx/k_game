
#define K_LOG_TAG "yx:object:player"
#include "k_log.h"

#include "object/player/yx_obj_player.h"

/* region [create] */

static int yx__obj_player_on_create_add_camera_follow(struct yx_obj_player *player) {

    struct k_camera_target *target = k_camera_add_follow_target(player->object, &player->x, &player->y);
    if (NULL == target)
        return -1;

    k_camera_set_primary_target(target);
    k_camera_set_target_weight(target, 4.0f);
    return 0;
}

struct yx_obj_player *yx_obj_player_create(const struct yx_obj_player_config *config) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_player));
    if (NULL == object)
        goto err;

    struct yx_obj_player *player = k_object_get_data(object);

    player->object = object;
    player->x = config->x;
    player->y = config->y;

    player->movement_state = YX__OBJ_PLAYER_STATE_IDLE;
    player->key_up    = 'W';
    player->key_left  = 'A';
    player->key_down  = 'S';
    player->key_right = 'D';
    player->speed     = 100.0f;

    player->spr_idle = config->spr_idle;
    player->spr_run  = config->spr_run;

    if (0 != yx__obj_player_on_create_add_draw(player))
        goto err;
    if (0 != yx__obj_player_on_create_add_movement(player))
        goto err;
    if (0 != yx__obj_player_on_create_add_camera_follow(player))
        goto err;
    if (0 != yx__obj_player_on_create_add_collision(player))
        goto err;
    if (0 != yx__obj_player_on_create_add_debug(player))
        goto err;

    return player;

err:
    k_object_destroy(object);
    k_log_error("failed to create yx_obj_player");
    return NULL;
}

/* endregion */
