
#define K_LOG_TAG "yx:object:player"
#include "k_log.h"

#include "sprite/yx_spr.h"
#include "object/fighter/player/yx_obj_player.h"
#include "object/weapon/apple/yx_obj_weapon_apple.h"
#include "config/yx_config_arena_blackboard.h"
#include "object/particle/yx_obj_shadow.h"

struct yx_obj_player *yx_obj_player_create(const struct yx_obj_player_config *config) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_player));
    if (NULL == object)
        goto err;

    struct yx_obj_player *player = k_object_get_data(object);

    player->object = object;
    player->x = config->x;
    player->y = config->y;

    player->key_up    = 'W';
    player->key_left  = 'A';
    player->key_down  = 'S';
    player->key_right = 'D';
    player->speed     = 200.0f;
    player->vx_movement  = 0.0f;
    player->vy_movement  = 0.0f;
    player->vx_knockback = 0.0f;
    player->vy_knockback = 0.0f;

    player->spr_idle = yx_spr_ynx_idle;
    player->spr_run  = yx_spr_ynx_run;

    if (0 != yx__obj_player_on_create_init_draw(player))
        goto err;
    if (0 != yx__obj_player_on_create_init_movement(player))
        goto err;
    if (0 != yx__obj_player_on_create_init_collision(player))
        goto err;
    if (0 != yx__obj_player_on_create_add_debug(player))
        goto err;

    player->weapon = yx_obj_player_weapon_apple_create();

    player->blackboard = k_room_blackboard_get(YX_ARENA_BLACKBOARD_KEY);
    if (NULL == player->blackboard) {
        k_log_error("object yx_obj_player requires room to provide blackboard '%s'", YX_ARENA_BLACKBOARD_KEY);
        goto err;
    }

    player->blackboard->player = player;

    {
        struct k_position_config position_config;
        position_config.world_x = &player->x;
        position_config.world_y = &player->y;
        position_config.parent = NULL;
        position_config.local_x = player->x;
        position_config.local_y = player->y;
        player->position = k_object_add_position(object, &position_config);
        if (NULL == player->position)
            goto err;
    }

    {
        player->object_shadow = yx_obj_shadow_create(yx_spr_shadow_1, player->position, 0, -15);
        if (NULL == player->object_shadow)
            goto err;
    }

    return player;

err:
    k_object_destroy(object);
    k_log_error("failed to create object yx_obj_player");
    return NULL;
}
