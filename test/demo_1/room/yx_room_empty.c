#include <limits.h>

#include "k_game.h"

#include "./yx_room_empty.h"

#include "../llk_ui/llk_ui.h"
#include "k_game/core/k_window.h"

static struct llk_ui_context *ui;
static const char *ui_xml = "./demo_1/ui/ui_2.xml";

static void yx__empty_room_ui_build(void) {

    ui = llk_ui_create_context();
    if (NULL == ui)
        return;

    struct llk_ui_elem *xml = llk_ui_build_elem_from_xml_file(ui, ui_xml);
    if (NULL == xml)
        goto err;

    struct llk_ui_elem *root = llk_ui_get_root(ui);
    if (0 != llk_ui_append_child(root, xml))
        goto err;

    return;

err:
    llk_ui_destroy_context(ui);
    ui = NULL;
}

static void yx__empty_room_ui_update(void *unused) {

    if (NULL != ui) {
        llk_ui_update(ui);
    }
}

static void yx__empty_room_ui_rebuild(void *unused) {
    (void)unused;

    if (k_key_down_or_pressed(K_KEY_LEFT_CTRL) && k_key_pressed(K_KEY_R)) {
        yx__empty_room_ui_build();
    }
}

static void yx__empty_room_ui_draw(void *unused) {

    if (NULL != ui) {
        llk_ui_draw(ui);
    }
}

static void draw_background(void *unused) {
    (void)unused;
    k_canvas_set_draw_color_rgba(0x1e1e1eff);
    k_canvas_room_clear();
}

static void set_window_top(void *unused) {
    if (k_key_down_or_pressed(K_KEY_LEFT_CTRL) && k_key_pressed(K_KEY_T)) {
        static int enabled = 1;
        k_window_set_always_on_top(enabled);
        enabled = !enabled;
    }
}

static int init_empty_room(void *unused) {
    (void)unused;

    k_room_add_draw_callback(NULL, draw_background, INT_MIN, 0);

    k_room_add_step_callback(NULL, set_window_top);

    yx__empty_room_ui_build();
    k_room_add_step_begin_callback(NULL, yx__empty_room_ui_update);
    k_room_add_step_callback(NULL, yx__empty_room_ui_rebuild);
    k_room_add_draw_callback(NULL, yx__empty_room_ui_draw, INT_MIN, 0);

    return 0;
}

struct k_room *yx_create_empty_room(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.fn_init = init_empty_room;

    return k_room_create(&config, NULL);
}
