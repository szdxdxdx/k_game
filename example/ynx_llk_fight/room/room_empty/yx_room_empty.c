#include <stdlib.h>

#include "k_game.h"
#include "k_webui.h"

#include "room/room_empty/yx_room_empty.h"

static int int_val;
static float float_val;
static int odd;
static void show_int_val(void *unused);

static int checkbox_change(void *data, int val) {
    odd = val;
    return 0;
}

static int text_read(void *data, struct k_str_buf *buf) {
    k_str_buf_clear(buf);

    k_str_buf_printf(buf, "%d", *(int *)data);
    return 0;
}

static void on_click(void *data) {
    k_webui_log_info("clicked");
}

static int checkbox_read(void *data, int *result) {
    *result = (odd % 2 != 0);
    return 0;
}

static int set_int_val(void *data, int val) {

    if (odd && val % 2 == 0)
        return -1;

    int *p_int = data;
    *p_int = val;
    return 0;
}

static int yx_room_empty_on_create(void *param) {

    k_room_add_draw_callback(NULL, show_int_val, INT_MIN, INT_MIN);

    {
        struct k_webui_text_config text = K_WEBUI_TEXT_CONFIG_INIT;
        text.on_read = text_read;
        k_webui_bind_text("group1", "文本框", &int_val, &text);
    }

    {
        struct k_webui_int_slider_config slider = K_WEBUI_INT_SLIDER_CONFIG_INIT;
        slider.max = 6;
        slider.on_input = set_int_val;
        k_webui_bind_int_slider("group1", "滑动条", &int_val, &slider);
    }

    {
        struct k_webui_float_slider_config slider = K_WEBUI_FLOAT_SLIDER_CONFIG_INIT;
        k_webui_bind_float_slider("group2", "滑动条", &float_val, &slider);
    }

    {
        struct k_webui_int_select_option options[] = {
            { .val=1, .text="Option 1" },
            { .val=2, .text="Option 2" },
            { .val=3, .text="Option 3" },
            { .val=4, .text="Option 4" },
            { .val=6, .text="Option 6" },
        };
        struct k_webui_int_select_config select = K_WEBUI_INT_SELECT_CONFIG_INIT;
        select.options = options;
        select.options_num = 5;
        k_webui_bind_int_select("group1", "下拉菜单", &int_val, &select);
    }

    {
        struct k_webui_checkbox_config checkbox = K_WEBUI_CHECKBOX_CONFIG_INIT;
        checkbox.on_change = checkbox_change;
        checkbox.on_read   = checkbox_read;
        k_webui_bind_checkbox("group1", "是奇数", NULL, &checkbox);
    }

    {
        struct k_webui_button_config button = K_WEBUI_BUTTON_CONFIG_INIT;
        button.on_click = on_click;
        k_webui_bind_button("group3", "按钮", NULL, &button);
    }

    return 0;
}

static void show_int_val(void *unused) {
    k_canvas_set_draw_color(0x000000ff);
    k_canvas_ui_clear();
    k_canvas_set_draw_color(0xffffffff);
    k_canvas_ui_printf(NULL, 8, 32 * 1, "%d", int_val);
    k_canvas_ui_printf(NULL, 8, 32 * 2, "%f", float_val);
}

struct k_room *yx_room_empty_create(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.on_create = yx_room_empty_on_create;

    return k_room_create(&config, NULL);
}
