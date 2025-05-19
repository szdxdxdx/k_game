
#define K_LOG_TAG "yx:object:rival"

#include <assert.h>
#include "k_log.h"

#include "k_game.h"
#include "config/yx_config_z_index.h"
#include "config/yx_config_arena_blackboard.h"
#include "object/fighter/rival/yx_obj_rival.h"
#include "object/fighter/player/yx_obj_player.h"

static void yx__obj_rival_on_step_end(struct k_object *object) {
    struct yx_obj_rival *rival = k_object_get_data(object);

    switch (rival->attack_state) {
        case YX_OBJ_RIVAL_STATE_PATROL: {
            k_sprite_renderer_flip_x(rival->spr_rdr, rival->face == -1 ? 0 : 1);
            break;
        }
        case YX_OBJ_RIVAL_STATE_ATTACK: {
            struct yx_obj_player *player = rival->blackboard->player;
            if (player->x < rival->x) {
                k_sprite_renderer_flip_x(rival->spr_rdr, 0);
            } else {
                k_sprite_renderer_flip_x(rival->spr_rdr, 1);
            }
            break;
        }
        default: {
            assert(0);
            return;
        }
    }
}

int yx__obj_rival_on_create_add_draw(struct yx_obj_rival *rival) {

    struct k_sprite_renderer_config renderer_config;
    renderer_config.x       = &rival->x;
    renderer_config.y       = &rival->y;
    renderer_config.sprite  = rival->spr_idle;
    renderer_config.z_group = YX_CONFIG_Z_GROUP_FIGHTER;
    renderer_config.z_layer = (int)rival->y;
    rival->spr_rdr = k_object_add_sprite_renderer(rival->object, &renderer_config);
    if (NULL == rival->spr_rdr)
        return -1;

    if (NULL == k_object_add_end_step_callback(rival->object, yx__obj_rival_on_step_end))
        return -1;

    return 0;
}
