
#include <stdlib.h>

#include "k_log.h"

#include "object/fighter/manager/yx_arena_manager.h"
#include "object/fighter/player/yx_obj_player.h"
#include "object/fighter/rival/yx_obj_rival.h"
#include "object/particle/yx_ui_banner.h"

struct yx_arena_manager *yx_arena_manager_create(void) {

    struct yx_arena_manager *manager = malloc(sizeof(struct yx_arena_manager));
    if (NULL == manager)
        goto err;

    manager->player = yx_obj_player_create();
    if (NULL == manager->player)
        goto err;

    manager->rival_wave_spawner = yx_obj_rival_wave_spawner_create();
    if (NULL == manager->rival_wave_spawner)
        goto err;

    manager->wave_count = 0;

    return manager;

err:
    // free(manager);
    return NULL;
}

void yx_arena_manager_notify_new_rival(struct yx_arena_manager *manager) {
    manager->rival_wave_spawner->rivals_num += 1;
}

static void yx__arena_manager_on_room_alarm_new_wave(void *manager_, int timeout_diff) {
    struct yx_arena_manager *manager = manager_;

    yx_obj_rival_wave_spawner_new_wave(10 + manager->wave_count * 5);
    manager->wave_count += 1;
}

static void yx__arena_manager_new_wave(struct yx_arena_manager *manager) {
    k_room_add_alarm_callback(manager,  yx__arena_manager_on_room_alarm_new_wave, 2000);
}

void yx_arena_manager_notify_rival_dead(struct yx_arena_manager *manager) {

    struct yx_obj_rival_wave_spawner *spawner = manager->rival_wave_spawner;
    spawner->rivals_num -= 1;

    if (0 == spawner->rivals_num) {
        if (manager->wave_count >= 3) {
            yx_ui_banner_show_YOU_WIN();
        } else {
            yx__arena_manager_new_wave(manager);
        }
    }
}

void yx_arena_manager_notify_enter_room(struct yx_arena_manager *manager) {
    yx__arena_manager_new_wave(manager);
}
