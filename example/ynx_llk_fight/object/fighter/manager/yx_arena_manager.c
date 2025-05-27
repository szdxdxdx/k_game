
#include <stdlib.h>

#include "object/fighter/manager/yx_arena_manager.h"
#include "object/fighter/player/yx_obj_player.h"
#include "object/fighter/rival/yx_obj_rival.h"

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

    return manager;

err:
    // free(manager);
    return NULL;
}

void yx_arena_manager_notify_new_rival(struct yx_arena_manager *manager) {
    manager->rival_wave_spawner->rivals_num += 1;
}

void yx_arena_manager_notify_rival_dead(struct yx_arena_manager *manager) {
    struct yx_obj_rival_wave_spawner *spawner = manager->rival_wave_spawner;
    spawner->rivals_num -= 1;
    if (0 == spawner->rivals_num) {
        yx_obj_rival_wave_spawner_new_wave(spawner, 10);
    }
}

void yx_arena_manager_notify_enter_room(struct yx_arena_manager *manager) {
    yx_obj_rival_wave_spawner_new_wave(manager->rival_wave_spawner, 10);
}
