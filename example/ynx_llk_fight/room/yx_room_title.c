
#include <limits.h>

#define K_LOG_TAG "yx:room:title"
#include "k_log.h"

#include "k_game.h"

#include "./yx_room_title.h"

#include "llk_ui.h"

#include "image/yx_image.h"
#include "sound/yx_sound.h"
#include "room/room_1/yx_room_1.h"

/* region [ui] */

static struct llk_ui_context *ui;

/* region [开始游戏的按钮] */

static void yx__room_title_ui_on_draw_button(struct llk_ui_elem *elem) {

    int is_hovered = llk_ui_elem_is_hovered(elem);
    int is_pressed = llk_ui_elem_is_pressed(elem);
    float x, y, w, h;
    llk_ui_elem_get_rect(elem, &x, &y, &w, &h);

    if (is_pressed) {
        k_canvas_ui_draw_image(img_ui_start_pushed, NULL, x, y, NULL);
    } else if (is_hovered) {
        k_canvas_ui_draw_image(img_ui_start_hovered, NULL, x, y, NULL);
    } else {
        k_canvas_ui_draw_image(img_ui_start_idle, NULL, x, y, NULL);
    }
}

static int lock = 0;

static void yx__room_title_on_alarm_goto_next_room(void *data, int timeout_diff) {
    k_room_nav_push(yx_room_1);
    lock = 0;
}

static void yx__room_title_goto_room_arena(void) {

    k_sound_sfx_play(yx_sfx_click);

    if (lock)
        return;
    else
        lock = 1;

    k_room_add_alarm_callback(NULL, yx__room_title_on_alarm_goto_next_room, 800);
}

/* endregion */

/* region [点击按钮，切换显示] */

static void yx__room_title_ui_btn1_on_click(struct llk_ui_elem *elem) {

    static int hidden = 1;
    hidden = !hidden;
    struct llk_ui_elem *panel = llk_ui_get_elem_by_id(ui, "area");
    llk_ui_elem_set_attr(panel, "hidden", hidden ? "false" : "true");
}

/* endregion */

/* region [] */

static int count = 4;

static void yx__room_title_ui_btn2_on_click(struct llk_ui_elem *elem) {
    switch (count) {
        case 4: { struct llk_ui_elem *box = llk_ui_get_elem_by_id(ui, "box4"); llk_ui_elem_set_attr(box, "hidden", "true"); count=3; break; }
        case 3: { struct llk_ui_elem *box = llk_ui_get_elem_by_id(ui, "box3"); llk_ui_elem_set_attr(box, "hidden", "true"); count=2; break; }
        case 2: { struct llk_ui_elem *box = llk_ui_get_elem_by_id(ui, "box2"); llk_ui_elem_set_attr(box, "hidden", "true"); count=1; break; }
        case 1: { struct llk_ui_elem *box = llk_ui_get_elem_by_id(ui, "box1"); llk_ui_elem_set_attr(box, "hidden", "true"); count=0; break; }
    }
}

static void yx__room_title_ui_btn3_on_click(struct llk_ui_elem *elem) {
    switch (count) {
        case 3: { struct llk_ui_elem *box = llk_ui_get_elem_by_id(ui, "box4"); llk_ui_elem_set_attr(box, "hidden", "false"); count=4; break; }
        case 2: { struct llk_ui_elem *box = llk_ui_get_elem_by_id(ui, "box3"); llk_ui_elem_set_attr(box, "hidden", "false"); count=3; break; }
        case 1: { struct llk_ui_elem *box = llk_ui_get_elem_by_id(ui, "box2"); llk_ui_elem_set_attr(box, "hidden", "false"); count=2; break; }
        case 0: { struct llk_ui_elem *box = llk_ui_get_elem_by_id(ui, "box1"); llk_ui_elem_set_attr(box, "hidden", "false"); count=1; break; }
    }
}

/* endregion */

static void yx__room_title_ui_on_click_goto_room_arena(struct llk_ui_elem *elem) {
    yx__room_title_goto_room_arena();
}

static void yx__room_title_build_ui(void) {

    if (NULL != ui) {
        llk_ui_destroy_context(ui);
    }

    ui = llk_ui_create_context();
    if (NULL == ui) {
        k_log_error("failed to build ui");
        goto err;
    }

    llk_ui_register_callback(ui, "goto_room_1", yx__room_title_ui_on_click_goto_room_arena);
    llk_ui_register_callback(ui, "ui_draw_start_game_button", yx__room_title_ui_on_draw_button);
    llk_ui_register_callback(ui, "btn1_on_click", yx__room_title_ui_btn1_on_click);
    llk_ui_register_callback(ui, "btn2_on_click", yx__room_title_ui_btn2_on_click);
    llk_ui_register_callback(ui, "btn3_on_click", yx__room_title_ui_btn3_on_click);

    struct llk_ui_elem *root = llk_ui_get_root(ui);
    struct llk_ui_elem *xml = llk_ui_build_elem_from_xml_file(ui, "demo_1/ui/ui_title.xml");
    if (NULL == xml) {
        k_log_error("failed to build ui from xml");
        goto err;
    }

    llk_ui_elem_append_child(root, xml);

    return;

err:
    if (NULL != ui) {
        llk_ui_destroy_context(ui);
        ui = NULL;
    }
}

static void yx__room_title_on_step_rebuild_ui(void *data) {
    if (k_key_down('B')) {
        yx__room_title_build_ui();
    }
}

static void yx__room_title_on_begin_step_update_ui(void *data) {
    if (NULL != ui) {
        llk_ui_update(ui);
    }
}

static void yx__room_title_on_draw_ui(void *data) {
    if (NULL != ui) {
        llk_ui_draw(ui);
    }
}

/* endregion */

static int yx__room_title_on_create(void *param) {

    yx__room_title_build_ui();

    k_room_add_step_callback(NULL, yx__room_title_on_step_rebuild_ui);

    k_room_add_step_callback(NULL, yx__room_title_on_begin_step_update_ui);
    k_room_add_draw_callback(NULL, yx__room_title_on_draw_ui, INT_MIN, 0);

    return 0;
}

struct k_room *yx_room_title;

struct k_room *yx_room_title_create(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.room_w = k_window_get_w();
    config.room_h = k_window_get_h();
    config.on_create = yx__room_title_on_create;

    struct k_room *room = k_room_create(&config, NULL);

    k_room_set_name(room, "title");

    yx_room_title = room;

    return room;
}
