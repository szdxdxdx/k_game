
#define K_LOG_TAG "yx:object:shadow"
#include "k_log.h"

#include "object/particle/yx_obj_shadow.h"

#include "config/yx_config_z_index.h"

struct yx_obj_shadow {
    float x;
    float y;
    struct k_sprite_renderer *spr_rdr;
    struct k_position *position;
};

static void yx__obj_shadow_on_end_step(struct k_object *object) {
    struct yx_obj_shadow *shadow = k_object_get_data(object);
    k_sprite_renderer_set_z_layer(shadow->spr_rdr, (int)shadow->y - 1);
}

struct k_object *yx_obj_shadow_create(struct k_sprite *sprite, struct k_position *position, float x, float y) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_shadow));
    if (NULL == object)
        goto err;

    struct yx_obj_shadow *shadow = k_object_get_data(object);

    {
        struct k_sprite_renderer_config config;
        config.x = &shadow->x;
        config.y = &shadow->y;
        config.sprite  = sprite;
        config.z_group = YX_CONFIG_Z_GROUP_FIGHTER;
        config.z_layer = 0;
        shadow->spr_rdr = k_object_add_sprite_renderer(object, &config);
        if (NULL == shadow->spr_rdr)
            goto err;
    }

    {
        struct k_position_config config;
        config.local_x = x;
        config.local_y = y;
        config.parent  = position;
        config.world_x = &shadow->x;
        config.world_y = &shadow->y;
        shadow->position = k_object_add_position(object, &config);
        if (NULL == shadow->position)
            goto err;
    }

    if (NULL == k_object_add_end_step_callback(object, yx__obj_shadow_on_end_step))
        goto err;

    return object;

err:
    k_log_error("failed to create object yx_obj_shadow");
    k_object_destroy(object);
    return NULL;
}
