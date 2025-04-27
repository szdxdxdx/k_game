#include "k_game.h"
#include "k_webui.h"

#include "./yx_room_empty.h"

static int int_val;

static void enter_room(void) {

    //k_webui_bind_int("test:a", &int_val);

}

struct k_room *yx_create_empty_room(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.fn_enter = enter_room;

    return k_room_create(&config, NULL);
}
