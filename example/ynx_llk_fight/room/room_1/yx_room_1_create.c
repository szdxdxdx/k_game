
#define K_LOG_TAG "yx:room:room_1"
#include "k_log.h"

#include "k_game.h"

#include "room/room_1/yx_room_1.h"

struct k_room *yx__room_1 = NULL;

static int arena_room_on_create(void *param) {
    (void)param;

    if (0 != yx__room_1_on_create_set_bkgd())
        return -1;
    if (0 != yx__room_1_on_create_place_obj())
        return -1;

    return 0;
}

struct k_room *yx_room_1_create(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.room_w     = 1920;
    config.room_h     = 1080;
    config.data_size  = sizeof(struct yx_room_1);
    config.on_create  = arena_room_on_create;

    struct k_room *room = k_room_create(&config, NULL);
    if (NULL == room)
        goto err;

    k_room_set_name(room, "room_1");

    yx__room_1 = room;
    return room;

err:

    k_log_error("failed to create room_1");
    return NULL;
}
