#include <stdio.h>
#include <limits.h>

#include "k_game.h"

#include "./yx_room_empty.h"

#include "../ui/yx_ui.h"

static void draw_background(void *unused) {
    (void)unused;
    k_canvas_set_draw_color_rgba(0x1e1e1eff);
    k_canvas_clear();
}

static struct yx_ui_context *ui;

static void init_ui(void) {

    // ui = yx_ui_create_context();

    // struct yx_ui_elem *button = yx_ui_create_elem(ui, "button");
    // yx_ui_set_attr(button, "x", "100");
    // yx_ui_set_attr(button, "y", "100");
    // yx_ui_set_attr(button, "w", "50");
    // yx_ui_set_attr(button, "h", "10");
    // yx_ui_set_attr(button, "background_color", "#ff6600ff");

    // struct yx_ui_elem *body = yx_ui_get_root(ui);
    // yx_ui_append_child(body, button);
}

static void draw_ui(void *unused) {
    /* 每帧手动调用绘制 ui */
    //yx_ui_draw(ui);
}

static int init_empty_room(void *params) {
    k_room_add_draw_callback(NULL, draw_background, INT_MIN, 0);
    k_room_add_draw_callback(NULL, draw_ui, INT_MAX, 0);
    init_ui();
    return 0;
}

struct k_room *yx_create_empty_room(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.fn_init = init_empty_room;

    return k_room_create(&config, NULL);
}
