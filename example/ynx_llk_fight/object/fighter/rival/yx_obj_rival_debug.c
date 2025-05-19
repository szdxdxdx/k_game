#include "k_game.h"

#include "config/yx_config_z_index.h"
#include "object/fighter/rival/yx_obj_rival.h"

static void yx__obj_rival_on_debug_draw(struct k_object *object) {
    struct yx_obj_rival *rival = k_object_get_data(object);

    k_canvas_set_draw_color(0xff00ffff);
    k_canvas_room_draw_line(rival->x, rival->y, rival->target_position_x, rival->target_position_y);
}

int yx__obj_rival_on_create_init_debug(struct yx_obj_rival *rival) {
   // k_object_add_draw_callback(rival->object, yx__obj_rival_on_debug_draw, YX_CONFIG_Z_GROUP_DEBUG, 0);
    return 0;
}
