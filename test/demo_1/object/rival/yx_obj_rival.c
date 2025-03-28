#include "../_internal.h"

struct k_object *yx_rival_create(const struct yx_obj_rival_config *config) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_rival));

    struct yx_obj_rival *rival = k_object_get_data(object);

    {
        struct k_position_config position_config;
        position_config.parent  = NULL;
        position_config.world_x = &rival->x;
        position_config.world_y = &rival->y;
        position_config.local_x = config->x;
        position_config.local_y = config->y;
        rival->position = k_object_add_position(object, &position_config);
    }

    {
        rival->spr_idle = config->spr_idle;
        rival->spr_run  = config->spr_run;

        struct k_sprite_renderer_config renderer_config;
        renderer_config.x       = &rival->x;
        renderer_config.y       = &rival->y;
        renderer_config.sprite  = rival->spr_idle;
        renderer_config.z_group = 0;
        renderer_config.z_layer = (int)config->y;
        rival->spr_rdr = k_object_add_sprite_renderer(object, &renderer_config);
    }

    return object;
}