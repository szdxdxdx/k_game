#include <limits.h>

#include "k_game.h"

#include "./yx_room_empty.h"

#include "../llk/llk_ui_context.h"
#include "../llk/llk_ui_elem.h"

static void draw_background(void *unused) {
    (void)unused;
    k_canvas_set_draw_color_rgba(0x1e1e1eff);
    k_canvas_clear();
}

static struct llk_ui_context *ui;

static void init_ui(void) {

    ui = llk_ui_create_context();

    struct llk_ui_elem *root = llk_ui_get_root(ui);

    struct llk_ui_elem *box_1 = llk_ui_create_elem(ui);
    box_1->w.unit = LLK_UI_UNIT_PX;
    box_1->w.val = 100.0f;
    box_1->h.unit = LLK_UI_UNIT_PX;
    box_1->h.val = 100.0f;
    box_1->background_color = 0x66ccffff;

    struct llk_ui_elem *box_2 = llk_ui_create_elem(ui);
    box_2->w.unit = LLK_UI_UNIT_PX;
    box_2->w.val = 100.0f;
    box_2->h.unit = LLK_UI_UNIT_PX;
    box_2->h.val = 100.0f;
    box_2->right.unit = LLK_UI_UNIT_PX;
    box_2->right.val = 0.0f;
    box_2->bottom.unit = LLK_UI_UNIT_PX;
    box_2->bottom.val = 0.0f;
    box_2->background_color = 0xee0000ff;

    llk_ui_append_child(root, box_1);
    llk_ui_append_child(root, box_2);
}

static void draw_ui(void *unused) {
    llk_ui_draw(ui);
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
