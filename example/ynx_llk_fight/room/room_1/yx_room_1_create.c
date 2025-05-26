
#define K_LOG_TAG "yx:room:room_1"

#include <limits.h>
#include "k_log.h"

#include "k_game.h"

#include "config/yx_config_arena_blackboard.h"
#include "config/yx_config_collision_group.h"
#include "room/room_1/yx_room_1.h"
#include "sound/yx_sound.h"

struct k_room *yx_room_1 = NULL;

static int yx__room_1_on_create(void *param) {
    (void)param;

    struct yx_room_1 *room_1 = k_room_get_data();
    room_1->draw_background = 1;

    if (0 != yx__room_1_on_create_set_bkgd())
        return -1;
    if (0 != k_room_add_blackboard())
        return -1;
    {
        struct yx_config_arena_blackboard *ctx = k_room_blackboard_add(YX_ARENA_BLACKBOARD_KEY, sizeof(struct yx_config_arena_blackboard));
        if (NULL == ctx)
            return -1;
        ctx->player = NULL;
    }
    if (0 != k_room_add_camera())
        return -1;
    if (0 != k_room_add_collision_manager())
        return -1;
    if (0 != yx__room_1_on_create_place_obj())
        return -1;

    /* tmp */

    return 0;
}

static void yx__room_1_on_enter(void) {
    k_sound_bgm_loop(yx_bgm_music, INT_MAX);
    // yx__room_1_on_enter_init_webui();
}

struct k_room *yx_room_1_create(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.room_w     = 640 * 2.0f;
    config.room_h     = 480 * 2.0f;
    config.data_size  = sizeof(struct yx_room_1);
    config.on_create  = yx__room_1_on_create;
    config.on_enter   = yx__room_1_on_enter;

    struct k_room *room = k_room_create(&config, NULL);
    if (NULL == room)
        goto err;

    k_room_set_name(room, "room_1");

    yx_room_1 = room;
    return room;

err:

    k_log_error("failed to create room_1");
    return NULL;
}
