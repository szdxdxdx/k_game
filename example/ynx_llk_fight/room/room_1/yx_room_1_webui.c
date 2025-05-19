#include "k_game.h"
#include "k_webui.h"

#include "k_str_buf.h"
#include "config/yx_config_collision_group.h"

#include "yx_room_1.h"

/* region [webui] */

#define WEBUI_GROUP "【示例 Demo】"

/* region [鼠标] */

static int webui_text_mouse_xy(void *data, struct k_str_buf *buf) {

    k_str_buf_printf(buf, "(%.2f, %.2f)\n", k_mouse_x(), k_mouse_y());
    return 0;
}

static void webui_add_text_bind_mouse_xy(void) {
    struct k_webui_text_config text = K_WEBUI_TEXT_CONFIG_INIT;
    text.on_read = webui_text_mouse_xy;
    k_webui_bind_text(WEBUI_GROUP, "鼠标坐标", NULL, &text);
}

/* endregion */

/* region [视野] */

static float view_scale = 1.0f;

static int webui_slider_set_view_scale(void *data, float val) {
    if (val < 0.1f)
        return -1;

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

static void webui_add_slider_bind_view_size(void) {
    struct k_webui_float_slider_config slider = K_WEBUI_FLOAT_SLIDER_CONFIG_INIT;
    slider.max   = 2.0f;
    slider.min   = 0.1f;
    slider.step  = 0.005f;
    slider.on_input = webui_slider_set_view_scale;
    slider.on_read  = webui_slider_get_view_scale;
    k_webui_bind_float_slider(WEBUI_GROUP, "视野大小", NULL, &slider);
}

/* endregion */

/* region [碰撞组] */

struct enable_collision_group {
    int group_id;
    int enable;
};

static struct enable_collision_group enable_collision_group_player_hp     = { .group_id=YX_CONFIG_COLLISION_GROUP_PLAYER_HP     , .enable=0 };
static struct enable_collision_group enable_collision_group_rival_hp      = { .group_id=YX_CONFIG_COLLISION_GROUP_RIVAL_HP      , .enable=0 };
static struct enable_collision_group enable_collision_group_player_bullet = { .group_id=YX_CONFIG_COLLISION_GROUP_PLAYER_BULLET , .enable=0 };
static struct enable_collision_group enable_collision_group_rival_bullet  = { .group_id=YX_CONFIG_COLLISION_GROUP_RIVAL_BULLET  , .enable=0 };

static int webui_checkbox_enable_collision_group_on_change(void *data, int checked) {
    struct enable_collision_group *enable = data;
    enable->enable = checked;
    k_collision_set_debug(enable->group_id, enable->enable);
    return 0;
}

static int webui_checkbox_enable_collision_group_on_read(void *data, int *result) {
    struct enable_collision_group *enable = data;
    *result = enable->enable;
    return 0;
}

static void webui_add_checkbox_bind_collision_group(void) {

    struct k_webui_checkbox_config checkbox = K_WEBUI_CHECKBOX_CONFIG_INIT;
    checkbox.on_change = webui_checkbox_enable_collision_group_on_change;
    checkbox.on_read   = webui_checkbox_enable_collision_group_on_read;
    k_webui_bind_checkbox(WEBUI_GROUP, "【碰撞盒】敌人", &enable_collision_group_rival_hp, &checkbox);
    k_webui_bind_checkbox(WEBUI_GROUP, "【碰撞盒】玩家", &enable_collision_group_player_hp, &checkbox);
    k_webui_bind_checkbox(WEBUI_GROUP, "【碰撞盒】敌人子弹", &enable_collision_group_rival_bullet, &checkbox);
    k_webui_bind_checkbox(WEBUI_GROUP, "【碰撞盒】玩家子弹", &enable_collision_group_player_bullet, &checkbox);
}

/* endregion */

/* region [绘制背景] */

static void webui_add_checkbox_bind_draw_background(void) {
    struct k_webui_checkbox_config checkbox = K_WEBUI_CHECKBOX_CONFIG_INIT;
    struct yx_room_1 *room_1 = k_room_get_data();
    k_webui_bind_checkbox(WEBUI_GROUP, "绘制房间背景", &room_1->draw_background, &checkbox);
}

/* endregion */

/* endregion */

int yx__room_1_on_create_init_webui(void) {
    webui_add_text_bind_mouse_xy();
    webui_add_slider_bind_view_size();
    webui_add_checkbox_bind_collision_group();
    webui_add_checkbox_bind_draw_background();

    return 0;
}
