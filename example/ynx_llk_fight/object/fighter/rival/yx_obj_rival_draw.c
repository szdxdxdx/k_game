
#define K_LOG_TAG "yx:object:rival"
#include "k_log.h"

#include "k_game.h"
#include "config/yx_config_z_index.h"
#include "object/fighter/rival/yx_obj_rival.h"

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

    return 0;
}
