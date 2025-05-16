
#define K_LOG_TAG "yx:room:room_1"
#include "k_log.h"

#include "k_game.h"

#include "room/yx_room_1.h"

static int yx__room_1_on_create(void *param) {

    return 0;
}

static void yx__room_1_on_destroy(void) {

}

static void yx__room_1_on_enter(void) {

}

struct k_room *yx_room_1 = NULL;

struct k_room *yx_room_1_create(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.room_w     = 1920;
    config.room_h     = 1080;
    config.data_size  = sizeof(struct yx_room_1);
    config.on_create  = yx__room_1_on_create;
    config.on_destroy = yx__room_1_on_destroy;
    config.on_enter   = yx__room_1_on_enter;

    struct k_room *room = k_room_create(&config, NULL);
    if (NULL == room) {
        k_log_error("failed to create room 1");
        return NULL;
    }

    yx_room_1 = room;
    return room;
}
