
#include "config/yx_config_z_index.h"
#include "sprite/yx_spr.h"
#include "object/particle_on_hit/yx_obj_particle_on_hit.h"

struct yx_obj_particle_on_hit *yx_obj_particle_on_hit_create(float x, float y) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_particle_on_hit));
    if (NULL == object)
        goto err;

    struct yx_obj_particle_on_hit *particle = k_object_get_data(object);
    particle->x = x;
    particle->y = y;

    {
        struct k_sprite_renderer_config spr_rdr_config;
        spr_rdr_config.x = &particle->x;
        spr_rdr_config.y = &particle->y;
        spr_rdr_config.sprite = yx_spr_particle_on_hit;
        spr_rdr_config.z_group = YX_CONFIG_Z_GROUP_TEXT;
        spr_rdr_config.z_layer = 0;
        struct k_sprite_renderer *spr_rdr = k_object_add_sprite_renderer(object, &spr_rdr_config);
        if (NULL == spr_rdr)
            goto err;

        k_sprite_renderer_set_loop_count(spr_rdr, 1);
        k_sprite_renderer_set_loop_callback(spr_rdr, k_object_destroy);
    }

    return particle;

err:
    k_object_destroy(object);
    return NULL;
}
