#include "k_game.h"
#include "k_webui.h"

#include "k_str_buf.h"

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

/* endregion */

int yx__room_1_on_create_init_webui(void) {
    webui_add_text_bind_mouse_xy();
    webui_add_slider_bind_view_size();

    return 0;
}
