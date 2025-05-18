#include <limits.h>
#include <math.h>
#include <stdio.h>

#define K_LOG_TAG "yx:room:arena"
#include "k_log.h"

#include "k_webui.h"
#include "llk_ui.h"
#include "k_game.h"


#include "sprite/yx_spr.h"

#include "object/bubble/yx_obj_bubble.h"
#include "object/fighter/player/yx_obj_player.h"
#include "object/fighter/rival/yx_obj_rival.h"

#include "room/yx_room_arena.h"
#include "object/bubble/yx_obj_bubble_maker.h"

/* region [webui] */

#define WEBUI_GROUP "【关卡一】"

static int webui_text_mouse_xy(void *data, struct k_str_buf *buf) {

    k_str_buf_printf(buf, "(%.2f, %.2f)\n", k_mouse_x(), k_mouse_y());
    return 0;
}

static void webui_text_bind_mouse_xy(void) {
    struct k_webui_text_config text = K_WEBUI_TEXT_CONFIG_INIT;
    text.on_read = webui_text_mouse_xy;
    k_webui_bind_text(WEBUI_GROUP, "鼠标坐标", NULL, &text);
}

static int show_ui = 1;
static int show_room = 1;

static void webui_checkbox_show_ui_or_room(void) {

    {
        struct k_webui_checkbox_config checkbox = K_WEBUI_CHECKBOX_CONFIG_INIT;
        k_webui_bind_checkbox(WEBUI_GROUP, "绘制 UI", &show_ui, &checkbox);
    }
    {
        struct k_webui_checkbox_config checkbox = K_WEBUI_CHECKBOX_CONFIG_INIT;
        k_webui_bind_checkbox(WEBUI_GROUP, "绘制房间", &show_room, &checkbox);
    }
}

static void clear_room_or_ui(void *data) {

    if ( ! show_ui) {
        k_canvas_set_draw_color(0x00000000);
        k_canvas_ui_clear();
    }

    if ( ! show_room) {
        k_canvas_set_draw_color(0x666666ff);
        k_canvas_room_clear();
    }
}

/* ------------------------------------------------------------------------ */

static float view_scale = 1.0f;

static int webui_slider_set_view_scale(void *data, float val) {

    if (val < 0.1f) {
        return -1;
    }

    view_scale = val;

    float vw = k_canvas_ui_get_vw();
    float vh = k_canvas_ui_get_vh();
    k_view_fit_rect(vw * view_scale, vh * view_scale);
    return 0;
}

static int webui_slider_get_view_scale(void *data, float *val) {
    *val = view_scale;
    return 0;
}

static void webui_slider_bind_view_size(void) {
    struct k_webui_float_slider_config slider = K_WEBUI_FLOAT_SLIDER_CONFIG_INIT;
    slider.max   = 2.0f;
    slider.min   = 0.1f;
    slider.step  = 0.005f;
    slider.on_input = webui_slider_set_view_scale;
    slider.on_read  = webui_slider_get_view_scale;
    k_webui_bind_float_slider(WEBUI_GROUP, "视野大小", NULL, &slider);
}

/* endregion */

/* region [ui] */

static const char *ui_xml_file_path = "./demo_1/ui/ui.xml";

static struct llk_ui_context *ui = NULL;

static void btn_on_click(struct llk_ui_elem *elem) {

    static int hidden = 0;
    hidden = !hidden;
    struct llk_ui_elem *panel = llk_ui_get_elem_by_id(ui, "state-panel");
    llk_ui_elem_set_attr(panel, "hidden", hidden ? "false" : "true");

    static int count = 5;
    if (0 == --count) {
        k_room_nav_pop();
    }
}

static void btn_on_draw(struct llk_ui_elem *elem) {

    float x, y, w, h;
    llk_ui_elem_get_rect(elem, &x, &y, &w, &h);

    if (llk_ui_elem_is_pressed(elem)) {
        k_canvas_set_draw_color(0x000000ff);
    } else {
        k_canvas_set_draw_color(0xffffffff);
    }

    k_canvas_ui_printf(NULL, x + 5, y + 5, "click me!");
}

static void slider_on_change(struct llk_ui_elem *elem, float old_val, float new_val) {
    printf("slider changed: %f -> %f\n", old_val, new_val);
}

static void room_build_ui(void) {

    if (NULL != ui) {
        llk_ui_destroy_context(ui);
    }

    ui = llk_ui_create_context();

    struct llk_ui_elem *root = llk_ui_get_root(ui);

    llk_ui_register_callback(ui, "click", btn_on_click);
    llk_ui_register_callback(ui, "draw", btn_on_draw);
    llk_ui_register_callback(ui, "on_change", slider_on_change);

    struct llk_ui_elem *box = llk_ui_elem_create(ui, "box");
    if (NULL == box)
        goto err;

    llk_ui_elem_set_attr(box, "background-color", "#ffffffaa");
    llk_ui_elem_set_attr(box, "w", "120");
    llk_ui_elem_set_attr(box, "h", "30");
    llk_ui_elem_set_attr(box, "top", "10");
    llk_ui_elem_set_attr(box, "right", "10");
    llk_ui_elem_set_attr(box, "on-click", "click");
    llk_ui_elem_set_attr(box, "on-draw", "draw");
    llk_ui_elem_append_child(root, box);

    struct llk_ui_elem *xml = llk_ui_build_elem_from_xml_file(ui, ui_xml_file_path);
    if (NULL == xml) {
        k_log_error("Failed to build ui from xml file");
    }

    llk_ui_elem_append_child(root, xml);

    return;

err:
    llk_ui_destroy_context(ui);
    ui = NULL;
}

static void room_update_ui(void *unused) {
    llk_ui_update(ui);
}

static void room_draw_ui(void *unused) {

    if (NULL == ui)
        return;

    k_canvas_set_draw_color(0x00000000);
    k_canvas_ui_clear();

    llk_ui_draw(ui);
}

/* endregion */

static void room_draw_background(void *unused) {

    k_canvas_set_draw_color(0x1e1e1eff);
    k_canvas_room_clear();

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

static void room_draw_text(void *unused) {

}

static void set_debug(void *data) {

    if (k_key_down('B')) {

        if (k_key_down_or_held(K_KEY_LEFT_SHIFT)) {
            k_camera_webui(0);
        }
        else {
            k_camera_webui(1);
        }
        if (k_key_down_or_held(K_KEY_LEFT_CTRL)) {
            room_build_ui();
        }
    }
}



static int arena_room_on_create(void *param) {
    (void)param;

    room_build_ui();

    struct yx_room_arena *room_arena = k_room_get_data();

    k_room_add_step_callback(NULL, set_debug);

    k_room_add_draw_callback(NULL, room_update_ui, INT_MIN, 0);
    k_room_add_draw_callback(NULL, room_draw_ui, INT_MIN, 0);

    k_room_add_draw_callback(NULL, room_draw_background, INT_MIN, 0);
    k_room_add_draw_callback(NULL, room_draw_text, INT_MIN, 1);

    k_room_add_draw_callback(NULL, clear_room_or_ui, K_DEBUG_Z_GROUP, K_DEBUG_Z_LAYER - 1);

    k_room_add_camera();

    k_room_add_collision_manager();

    {
        yx_obj_bubble_maker_create();
    }

    {
        struct yx_obj_player_config config;
        config.x = 300;
        config.y = 400;
        room_arena->player = yx_obj_player_create(&config);
    }

    {
        struct yx_obj_rival_config config;
        config.x = 800;
        config.y = 600;
        yx_obj_rival_create(&config);
    }

    {
        struct yx_obj_rival_config config;
        config.x = 400;
        config.y = 300;
        yx_obj_rival_create(&config);
    }

    {
        struct yx_obj_rival_config config;
        config.x = 600;
        config.y = 500;
        yx_obj_rival_create(&config);
    }

    return 0;
}

static void arena_room_on_destroy(void) {

    llk_ui_destroy_context(ui);
}

static void arena_room_on_enter(void) {
    // k_window_set_always_on_top(1);
    webui_text_bind_mouse_xy();
    webui_slider_bind_view_size();
    webui_checkbox_show_ui_or_room();
}

struct k_room *yx_room_arena_create(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.room_w     = 1920;
    config.room_h     = 1080;
    config.data_size  = sizeof(struct yx_room_arena);
    config.on_create  = arena_room_on_create;
    config.on_destroy = arena_room_on_destroy;
    config.on_enter   = arena_room_on_enter;

    struct k_room *room = k_room_create(&config, NULL);

    k_room_set_name(room, "arena");

    return room;
}
