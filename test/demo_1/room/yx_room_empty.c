#include <stdio.h>
#include <limits.h>

#include "k_game.h"

#include "./yx_room_empty.h"

#include "../sprite/yx_spr.h"
#include "../object/yx_obj_bubble.h"

static void room_ui(void *data) {

    k_canvas_draw_sprite(yx_spr_bubble_float, 0, 160, 100, NULL);
}

static void enter_empty_room(void) {


}

static int init_empty_room(void *params) {
    k_room_add_draw_callback(NULL, room_ui, 0, 0);

    k_room_add_draw_callback(NULL, k_clean_room_canvas, INT_MIN, 0);

    yx_create_bubble(100, 100);

    return 0;
}

struct k_room *yx_create_empty_room(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.fn_init  = init_empty_room;
    config.fn_enter = enter_empty_room;

    return k_room_create(&config, NULL);
}
