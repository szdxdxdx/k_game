
#include <limits.h>

#define K_LOG_TAG "yx:room:room_1"
#include "k_log.h"

#include "k_game.h"

#include "config/yx_config_arena_blackboard.h"
#include "config/yx_config_collision_group.h"
#include "object/fighter/player/yx_obj_player.h"
#include "object/fighter/rival/yx_obj_rival.h"
#include "object/fighter/manager/yx_arena_manager.h"
#include "room/room_1/yx_room_1.h"
#include "sound/yx_sound.h"

struct k_room *yx_room_1 = NULL;

static void yx__room_1_on_debug_step(void *data) {
    if (k_key_down('B'))
        k_camera_shake(16.0f, 0.5f);
    if (k_key_down_or_held('N'))
        k_camera_shake(16.0f, 0.5f);
}

static int yx__room_1_on_create(void *param) {
    (void)param;

    k_room_add_step_callback(NULL, yx__room_1_on_debug_step);

    struct yx_room_1 *room_1 = k_room_get_data();
    room_1->draw_background = 1;

    if (0 != yx__room_1_on_create_set_bkgd())
        return -1;
    if (0 != k_room_add_blackboard())
        return -1;
    if (0 != k_room_add_camera())
        return -1;
    if (0 != k_room_add_collision_manager())
        return -1;
    if (0 != yx__room_1_init_ui())
        return -1;

    struct yx_config_arena_blackboard *blackboard = k_room_blackboard_add(YX_ARENA_BLACKBOARD_KEY, sizeof(struct yx_config_arena_blackboard));
    if (NULL == blackboard)
        return -1;
    room_1->arena_blackboard = blackboard;

    blackboard->manager = yx_arena_manager_create();
    if (NULL == blackboard->manager)
        return -1;

    return 0;
}

static void yx__room_1_on_enter(void) {
    //k_camera_set_debug_draw_enabled(1);
    yx__room_1_on_enter_init_webui();

    k_sound_bgm_loop(yx_bgm_music, INT_MAX);

    struct yx_config_arena_blackboard *blackboard = k_room_blackboard_get(YX_ARENA_BLACKBOARD_KEY);
    float player_x = blackboard->manager->player->x;
    float player_y = blackboard->manager->player->y;
    k_camera_set_view_position(player_x, player_y);

    yx_arena_manager_notify_enter_room(blackboard->manager);
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
