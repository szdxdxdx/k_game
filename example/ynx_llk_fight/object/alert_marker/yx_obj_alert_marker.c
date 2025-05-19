
#include "config/yx_config_z_index.h"
#include "sprite/yx_spr.h"
#include "object/alert_marker/yx_obj_alert_marker.h"

static void yx__obj_alert_marker_on_alarm(struct k_object *object, int timeout_diff) {
    k_object_destroy(object);
}

struct yx_obj_alert_marker *yx_obj_alert_marker_create(struct k_position *position, float offset_x, float offset_y) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_alert_marker));
    if (NULL == object)
        goto err;

    struct yx_obj_alert_marker *marker = k_object_get_data(object);

    {
        struct k_sprite_renderer_config spr_rdr_config;
        spr_rdr_config.x = &marker->x;
        spr_rdr_config.y = &marker->y;
        spr_rdr_config.sprite = yx_spr_alert_marker;
        spr_rdr_config.z_group = YX_CONFIG_Z_GROUP_TEXT;
        spr_rdr_config.z_layer = 0;
        struct k_sprite_renderer *spr_rdr = k_object_add_sprite_renderer(object, &spr_rdr_config);
        if (NULL == spr_rdr)
            goto err;

        k_sprite_renderer_set_loop_count(spr_rdr, 1);
    }

    {
        struct k_position_config pos_config;
        pos_config.parent = position;
        pos_config.world_x = &marker->x;
        pos_config.world_y = &marker->y;
        pos_config.local_x = offset_x;
        pos_config.local_y = offset_y;
        struct k_position *pos = k_object_add_position(object, &pos_config);
        if (NULL == pos)
            goto err;
    }

    if (NULL == k_object_add_alarm_callback(object, yx__obj_alert_marker_on_alarm, 2000))
        goto err;

    return marker;

err:
    k_object_destroy(object);
    return NULL;
}
