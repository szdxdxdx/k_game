#include <limits.h>
#include <math.h>
#include <stdio.h>

#include "./yx_room_arena.h"

#include "../sprite/yx_spr.h"
#include "../object/yx_obj.h"

#include "../llk_ui/llk_ui_context.h"
#include "../llk_ui/llk_ui_elem.h"
#include "../llk_ui/llk_ui_build.h"

/* region [ui] */

static void on_click(void) {
    printf("clicked\n");
}

static struct llk_ui_context *ui = NULL;

static void room_build_ui(void) {

    if (NULL != ui) {
        llk_ui_destroy_context(ui);
    }

    ui = llk_ui_create_context();
    llk_ui_register_callback(ui, "click", on_click);

    struct llk_ui_elem *box = llk_ui_create_elem(ui, "box");

    llk_ui_elem_set_attr(box, "background-color", "#ffffffaa");
    llk_ui_elem_set_attr(box, "w", "60");
    llk_ui_elem_set_attr(box, "h", "10");
    llk_ui_elem_set_attr(box, "top", "10");
    llk_ui_elem_set_attr(box, "right", "10");
    llk_ui_append_child(llk_ui_get_root(ui), box);

    struct llk_ui_elem *xml = llk_ui_build_elem_from_xml_file(ui, "demo_1/ui/ui.xml");
    llk_ui_append_child(llk_ui_get_root(ui), xml);
}

static void room_update_ui(void *unused) {
    llk_ui_update(ui);
}

static void room_draw_ui(void *unused) {
    k_canvas_set_draw_color_rgba(0x00000000);
    k_canvas_ui_clear();

    llk_ui_draw(ui);
}

/* endregion */

/* region [room_background] */

static void room_draw_background(void *unused) {

    k_canvas_set_draw_color_rgba(0x1e1e1eff);
    k_canvas_room_clear();

    float view_x;
    float view_y;
    float view_w;
    float view_h;
    k_view_get_rect(&view_x, &view_y, &view_w, &view_h);

    float grid_size = 48;

    float w = k_room_get_width();
    float h = k_room_get_height();

    k_canvas_set_draw_color_rgba(0x323333ff);

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

/* endregion */

/* region [room_draw] */

/* endregion */

static void set_debug(void *data) {

    if (k_key_pressed('B')) {

        if (k_key_down(K_KEY_LEFT_SHIFT)) {
            k_collision_set_debug(YX_COLLISION_GROUP_BUBBLE, 0);
            k_camera_set_debug(0);
        }
        else {
            k_collision_set_debug(YX_COLLISION_GROUP_BUBBLE, 1);
            k_camera_set_debug(1);
        }
        if (k_key_down(K_KEY_LEFT_CTRL)) {
            room_build_ui();
        }
    }
}

static int init_arena_room(void *params) {
    (void)params;

    room_build_ui();

    struct yx_room_arena *room_arena = k_room_get_data();

    k_room_add_step_callback(NULL, set_debug);

    k_room_add_draw_callback(NULL, room_update_ui, INT_MIN, 0);
    k_room_add_draw_callback(NULL, room_draw_ui, INT_MIN, 0);

    k_room_add_draw_callback(NULL, room_draw_background, INT_MIN, 0);

    k_room_add_camera();
    k_room_add_collision_manager();

    {
        struct yx_obj_bubble_maker_config config;
        yx_create_bubble_maker(&config);
    }

    {
        struct yx_obj_player_config config;
        config.x = 300;
        config.y = 400;
        config.spr_idle = yx_spr_ynx_idle;
        config.spr_run  = yx_spr_ynx_run;
        room_arena->player = yx_create_player(&config);
    }

    {
        struct yx_obj_rival_config config;
        config.x = 800;
        config.y = 600;
        config.spr_idle = yx_spr_liliko_idle;
        config.spr_run  = yx_spr_liliko_run;
        yx_create_rival(&config);
    }

    {
        struct yx_obj_rival_config config;
        config.x = 400;
        config.y = 300;
        config.spr_idle = yx_spr_liliko_idle;
        config.spr_run  = yx_spr_liliko_run;
        yx_create_rival(&config);
    }

    {
        struct yx_obj_rival_config config;
        config.x = 600;
        config.y = 500;
        config.spr_idle = yx_spr_liliko_idle;
        config.spr_run  = yx_spr_liliko_run;
        yx_create_rival(&config);
    }

    return 0;
}

static void enter_arena_room(void) {
    // k_window_set_always_on_top(1);
}

struct k_room *yx_create_arena_room(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.room_w    = 1920 * 1.5f;
    config.room_h    = 1080 * 1.5f;
    config.data_size = sizeof(struct yx_room_arena);
    config.fn_init   = init_arena_room;
    config.fn_enter  = enter_arena_room;

    return k_room_create(&config, NULL);
}
