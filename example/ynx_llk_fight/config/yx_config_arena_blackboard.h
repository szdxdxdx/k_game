
#ifndef YX_CONFIG_ARENA_BLACKBOARD_H
#define YX_CONFIG_ARENA_BLACKBOARD_H

struct yx_obj_player;
struct yx_obj_rival;

struct yx_config_arena_blackboard {
    struct yx_obj_player *player;
    struct yx_obj_rival  *rival_1;
    struct yx_obj_rival  *rival_2;
    struct yx_obj_rival  *rival_3;
};

#endif
