#ifndef YX_ARENA_MANAGER_H
#define YX_ARENA_MANAGER_H

struct yx_obj_player;
struct yx_obj_rival_wave_spawner;

struct yx_arena_manager {
    struct yx_obj_player *player;
    struct yx_obj_rival_wave_spawner *rival_wave_spawner;
};

struct yx_arena_manager *yx_arena_manager_create(void);

void yx_arena_manager_notify_new_rival(struct yx_arena_manager *manager);

void yx_arena_manager_notify_rival_dead(struct yx_arena_manager *manager);

void yx_arena_manager_notify_enter_room(struct yx_arena_manager *manager);

#endif
