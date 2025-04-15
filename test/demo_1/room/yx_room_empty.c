#include <stdio.h>
#include <limits.h>

#include "k_game.h"

#include "./yx_room_empty.h"

#include "../sprite/yx_spr.h"
#include "../object/yx_obj_bubble.h"

static void draw_background(void *unused) {
    (void)unused;
    k_canvas_set_draw_color_rgba(0x1e1e1eff);
    k_canvas_clear();
}

static void room_ui(void *data) {

}

static int init_empty_room(void *params) {
    k_room_add_draw_callback(NULL, draw_background, INT_MIN, 0);
    k_room_add_draw_callback(NULL, room_ui, 0, 0);
    return 0;
}

struct k_room *yx_create_empty_room(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.fn_init = init_empty_room;

    return k_room_create(&config, NULL);
}
