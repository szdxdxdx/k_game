
#include <limits.h>

#define K_LOG_TAG "yx:room:room_1"
#include "k_log.h"

#include "k_game.h"
#include "llk_ui.h"

#include "config/yx_config_z_index.h"
#include "config/yx_config_arena_blackboard.h"
#include "config/yx_config_collision_group.h"
#include "object/fighter/player/yx_obj_player.h"
#include "object/fighter/rival/yx_obj_rival.h"
#include "object/fighter/manager/yx_arena_manager.h"
#include "room/room_1/yx_room_1.h"

static struct llk_ui_context *ui;

static void yx__room_1_build_ui(void) {

    if (NULL != ui) {
        llk_ui_destroy_context(ui);
    }

    ui = llk_ui_create_context();
    if (NULL == ui) {
        k_log_error("failed to build ui");
        goto err;
    }

    struct llk_ui_elem *root = llk_ui_get_root(ui);
    struct llk_ui_elem *xml = llk_ui_build_elem_from_xml_file(ui, "demo_1/ui/ui_room_1.xml");
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

static void yx__room_1_on_step_rebuild_ui(void *data) {
    if (k_key_down('K')) {
        yx__room_1_build_ui();
    }
}

static void yx__room_1_on_begin_step_update_ui(void *data) {
    if (NULL != ui) {
        llk_ui_update(ui);
    }
}

static void yx__room_1_on_draw_ui(void *data) {

    k_canvas_set_draw_color(0x00000000);
    k_canvas_ui_clear();

    if (NULL != ui) {
        llk_ui_draw(ui);
    }
}

int yx__room_1_init_ui(void) {

    yx__room_1_build_ui();

    k_room_add_begin_step_callback(NULL, yx__room_1_on_begin_step_update_ui);
    k_room_add_draw_callback(NULL, yx__room_1_on_draw_ui, YX_CONFIG_Z_GROUP_BKGD, YX_CONFIG_Z_LAYER_BKGD);

    /* debug */
    k_room_add_step_callback(NULL, yx__room_1_on_step_rebuild_ui);

    return 0;
}
