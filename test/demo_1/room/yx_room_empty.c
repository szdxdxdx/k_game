#include <limits.h>

#include "k_game.h"

#include "./yx_room_empty.h"

#include "../llk/llk_ui_context.h"
#include "../llk/llk_ui_elem.h"

static void draw_background(void *unused) {
    (void)unused;
    k_canvas_set_draw_color_rgba(0x1e1e1eff);
    k_canvas_room_clear();
}

static struct llk_ui_context *ui;

static void init_ui(void) {

    ui = llk_ui_create_context();

    struct llk_ui_elem *root = llk_ui_get_root(ui);

    struct llk_ui_elem *my_root = llk_ui_create_elem(ui);
    my_root->w.unit = LLK_UI_UNIT_PERCENT;
    my_root->w.specified_val = 1.0f;
    my_root->h.unit = LLK_UI_UNIT_PERCENT;
    my_root->h.specified_val = 1.0f;
    my_root->background_color = 0xffffffff;
    llk_ui_append_child(root, my_root);

    struct llk_ui_elem *box_1 = llk_ui_create_elem(ui);
    box_1->w.unit = LLK_UI_UNIT_PX;
    box_1->w.specified_val = 100.0f;
    box_1->h.unit = LLK_UI_UNIT_PX;
    box_1->h.specified_val = 100.0f;
    box_1->background_color = 0x66ccffff;
    llk_ui_append_child(my_root, box_1);

    struct llk_ui_elem *box_2 = llk_ui_create_elem(ui);
    box_2->w.unit = LLK_UI_UNIT_PX;
    box_2->w.specified_val = 100.0f;
    box_2->h.unit = LLK_UI_UNIT_PX;
    box_2->h.specified_val = 100.0f;
    box_2->right.unit = LLK_UI_UNIT_PERCENT;
    box_2->right.specified_val = 0.5f;
    box_2->bottom.unit = LLK_UI_UNIT_PX;
    box_2->bottom.specified_val = 0.0f;
    box_2->background_color = 0xee0000ff;
    llk_ui_append_child(my_root, box_2);

    struct llk_ui_elem *box_3 = llk_ui_create_elem(ui);
    box_3->w.unit = LLK_UI_UNIT_PX;
    box_3->w.specified_val = 30.0f;
    box_3->h.unit = LLK_UI_UNIT_PERCENT;
    box_3->h.specified_val = 1.0f;
    box_3->right.unit = LLK_UI_UNIT_PX;
    box_3->right.specified_val = 10.0f;
    box_3->background_color = 0xff0099ff;
    llk_ui_append_child(my_root, box_3);
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
