#include <stdio.h>
#include <stdlib.h>

#include "k_game.h"
#include "k_webui.h"

#include "./yx_room_empty.h"

static int int_val;
static int int_val2;
static int int_val3;
static float float_val;

static void show_int_val(void *unused) {
    k_canvas_set_draw_color_rgba(0x000000ff);
    k_canvas_ui_clear();

    k_canvas_set_draw_color_rgba(0xffffffff);
    k_canvas_ui_printf(NULL, 8, 32 * 1, "%d", int_val);
    k_canvas_ui_printf(NULL, 8, 32 * 2, "%d", int_val2);
    k_canvas_ui_printf(NULL, 8, 32 * 3, "%d", int_val3);
    k_canvas_ui_printf(NULL, 8, 32 * 4, "%f", float_val);
}

static int init(void *data, void *param) {
    *((int **)data) = (int *)param;
    return 0;
}

static int webui_set(void *data, const char *val) {
    int *p = *(int **)data;

    printf("%s", val);

    *p = atoi(val);
    return 0;
}

static void enter_room(void) {

    k_room_add_draw_callback(NULL, show_int_val, INT_MIN, INT_MIN);

    k_webui_log_info("测试 - %s", "输出一条日志");


    k_webui_bind_int("int_val", &int_val, NULL);

    {
        struct k_webui_int_options options;
        options.input_type = K_WEBUI_INT_RANGE;
        options.range.max = 100;
        options.range.min = 0;
        options.range.step = 3;
        k_webui_bind_int("int range", &int_val2, &options);
    }

    {
        struct k_webui_int_options options;
        options.input_type = K_WEBUI_CHECKBOX;
        options.checkbox._ = NULL;
        k_webui_bind_int("check box", &int_val3, &options);
    }

    {
        struct k_webui_float_options options;
        options.input_type = K_WEBUI_FLOAT_RANGE;
        options.range.max  = 2.0f;
        options.range.min  = 1.0f;
        options.range.step = 0.02f;
        k_webui_bind_float("float range", &float_val, &options);
    }

    //k_webui_bind_int("test:a", &int_val);

}

struct k_room *yx_create_empty_room(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.fn_enter = enter_room;

    return k_room_create(&config, NULL);
}
