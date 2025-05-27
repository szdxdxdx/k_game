#ifndef YX_CONFIG_ARENA_BLACKBOARD_H
#define YX_CONFIG_ARENA_BLACKBOARD_H

struct yx_arena_manager;

#define YX_ARENA_BLACKBOARD_KEY "yx_config_arena_blackboard"

struct yx_config_arena_blackboard {
    struct yx_arena_manager *manager;
};

#endif
