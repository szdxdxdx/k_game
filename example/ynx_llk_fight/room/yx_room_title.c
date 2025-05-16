#define K_LOG_TAG "yx:room:title"

#include <limits.h>
#include "k_log.h"

#include "k_game.h"

#include "./yx_room_title.h"

#include "../llk_ui/llk_ui.h"

/* region [goto_room_arena] */

static void yx__room_title_goto_room_arena(void) {
    k_room_nav_push(k_room_find("arena"));
}

/* endregion */

/* region [ui] */

static struct llk_ui_context *ui;

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

struct k_room *yx_room_title_create(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.room_w = k_window_get_w();
    config.room_h = k_window_get_h();
    config.on_create = yx__room_title_on_create;

    struct k_room *room = k_room_create(&config, NULL);

    k_room_set_name(room, "title");

    return room;
}
