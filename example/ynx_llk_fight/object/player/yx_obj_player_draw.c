
#define K_LOG_TAG "yx:object:player"
#include "k_log.h"

#include "k_game.h"

#include "object/player/yx_obj_player.h"
#include "config/yx_config_z_index.h"

int yx__obj_player_on_create_add_draw(struct yx_obj_player *player) {

    struct k_sprite_renderer_config renderer_config;
    renderer_config.x       = &player->x;
    renderer_config.y       = &player->y;
    renderer_config.sprite  = player->spr_idle;
    renderer_config.z_group = YX_CONFIG_Z_GROUP_FIGHTER;
    renderer_config.z_layer = (int)player->y;
    player->spr_rdr = k_object_add_sprite_renderer(player->object, &renderer_config);
    if (NULL == player->spr_rdr)
        return -1;

    return 0;
}
