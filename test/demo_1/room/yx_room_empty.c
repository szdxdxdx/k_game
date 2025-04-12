#include "k_game/core/k_room.h"

#include "./yx_room_empty.h"

static int init_empty_room(void *params) {

    return 0;
}

static void enter_empty_room(void) {

}

struct k_room *yx_create_empty_room(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.fn_init  = init_empty_room;
    config.fn_enter = enter_empty_room;

    return k_room_create(&config, NULL);
}
