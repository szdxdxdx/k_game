
#include <stdlib.h>

#define K_LOG_TAG "yx:object:rival"
#include "k_log.h"

#include "k_game.h"

#include "sprite/yx_spr.h"

#include "object/fighter/rival/yx_obj_rival.h"
#include "config/yx_config_arena_blackboard.h"

struct yx_obj_rival *yx_obj_rival_create(const struct yx_obj_rival_config *config) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_rival));

    struct yx_obj_rival *rival = k_object_get_data(object);
    rival->object = object;

    rival->x = config->x;
    rival->y = config->y;
    rival->vx_movement  = 0.0f;
    rival->vy_movement  = 0.0f;
    rival->vx_knockback = 0.0f;
    rival->vy_knockback = 0.0f;
    rival->spr_idle = yx_spr_llk_idle;
    rival->spr_run  = yx_spr_llk_run;
    rival->face = (rand() % 2) ? -1 : 1;
    rival->attack_state = YX_OBJ_RIVAL_STATE_PATROL;

    if (0 != yx__obj_rival_on_create_add_movement(rival))
        goto err;
    if (0 != yx__obj_rival_on_create_add_collision(rival))
        goto err;
    if (0 != yx__obj_rival_on_create_add_draw(rival))
        goto err;
    if (0 != yx__obj_rival_on_create_add_debug(rival))
        goto err;

    {
        struct k_position_config position_config;
        position_config.parent  = NULL;
        position_config.world_x = &rival->x;
        position_config.world_y = &rival->y;
        position_config.local_x = rival->x;
        position_config.local_y = rival->y;
        rival->position = k_object_add_position(object, &position_config);
        if (NULL == rival->position)
            goto err;
    }

    rival->blackboard = k_room_blackboard_get(YX_ARENA_BLACKBOARD_KEY);
    if (NULL == rival->blackboard) {
        k_log_error("object yx_obj_rival requires room to provide blackboard '%s'", YX_ARENA_BLACKBOARD_KEY);
        goto err;
    }

    return rival;

err:
    k_object_destroy(object);
    k_log_error("failed to create object yx_obj_rival");
    return NULL;
}
