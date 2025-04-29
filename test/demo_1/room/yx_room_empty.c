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

    {
        struct k_webui_widget_config widget;
        widget.input_type = K_WEBUI_INT_SLIDER;
        widget.as_int_slider.min  = 1;
        widget.as_int_slider.max  = 3;
        widget.as_int_slider.step = 1;
        widget.as_int_slider.on_input = NULL;
        widget.as_int_slider.on_read  = NULL;
        k_webui_bind("range", &int_val, &widget);
    }

    {
        struct k_webui_int_select_option options[] = {
            { .val=1, .text="Option 1" },
            { .val=2, .text="Option 2" },
            { .val=3, .text="Option 3" },
        };
        struct k_webui_widget_config widget;
        widget.input_type = K_WEBUI_INT_SELECT;
        widget.as_int_select.count     = 3;
        widget.as_int_select.options   = options;
        widget.as_int_select.on_change = NULL;
        widget.as_int_slider.on_read   = NULL;
        k_webui_bind("Select", &int_val, &widget);
    }
}

struct k_room *yx_create_empty_room(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.fn_enter = enter_room;

    return k_room_create(&config, NULL);
}
