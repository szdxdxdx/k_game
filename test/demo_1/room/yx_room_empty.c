#include <stdlib.h>

#include "k_game.h"
#include "k_webui.h"

#include "./yx_room_empty.h"

static int int_val;

static void show_int_val(void *unused) {
    k_canvas_set_draw_color_rgba(0x000000ff);
    k_canvas_ui_clear();
    k_canvas_set_draw_color_rgba(0xffffffff);
    k_canvas_ui_printf(NULL, 8, 32 * 1, "%d", int_val);
}

static int set_val(void *p, int val) {
    *(int *)p = val;
    return 0;
}

static void enter_room(void) {

    k_room_add_draw_callback(NULL, show_int_val, INT_MIN, INT_MIN);

    struct k_webui_widget_config widget;
    widget.input_type = K_WEBUI_INT_RANGE;
    widget.as_int_range.min  = 100;
    widget.as_int_range.max  = 200;
    widget.as_int_range.step = 2;
    widget.as_int_range.on_input = NULL;
    k_webui_bind("range", &int_val, &widget);
}

struct k_room *yx_create_empty_room(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.fn_enter = enter_room;

    return k_room_create(&config, NULL);
}
