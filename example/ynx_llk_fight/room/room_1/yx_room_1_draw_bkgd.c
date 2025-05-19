#include <math.h>

#include "k_game.h"

#include "config/yx_config_z_index.h"

#include "room/room_1/yx_room_1.h"
#include "image/yx_image.h"

static void yx__room_1_draw_grid(void) {

    float view_x;
    float view_y;
    float view_w;
    float view_h;
    k_view_get_rect(&view_x, &view_y, &view_w, &view_h);

    float grid_size = 48;

    float w = k_room_get_w();
    float h = k_room_get_h();

    k_canvas_set_draw_color(0x323333ff);

    float x = floorf(view_x / grid_size) * grid_size;
    float x_to = ceilf((view_x + view_w) / grid_size) * grid_size;
    while (x < x_to) {
        k_canvas_room_draw_line(x, 0, x, h);
        x += grid_size;
    }

    float y = floorf(view_y / grid_size) * grid_size;
    float y_to = ceilf((view_y + view_h) / grid_size) * grid_size;
    while (y < y_to) {
        k_canvas_room_draw_line(0, y, w, y);
        y += grid_size;
    }
}

static void yx__room_1_draw_background(void *data) {
    (void)data;

    k_canvas_set_draw_color(0x00000000);
    k_canvas_ui_clear();

    struct yx_room_1 *room_1 = k_room_get_data();
    if (room_1->draw_background) {
        k_canvas_room_draw_image(img_bkgd, NULL, 0, 0, NULL);
    }
    else {
        k_canvas_set_draw_color(0x1e1e1eff);
        k_canvas_room_clear();
        yx__room_1_draw_grid();
    }
}

int yx__room_1_on_create_set_bkgd(void) {
    k_room_add_draw_callback(NULL, yx__room_1_draw_background, YX_CONFIG_Z_GROUP_BKGD, YX_CONFIG_Z_LAYER_BKGD);
    return 0;
}
