#ifndef K_GAME_H
#define K_GAME_H

#include <stddef.h>
#include <stdint.h>

struct k_game_config;
struct k_room_config;

struct k_room_context;

int k_run_game(const struct k_game_config *config);

struct k_game_config {

    int window_w;
    int window_h;

    int (*fn_setup_game)(void);
};

extern const struct k_game_config K_GAME_CONFIG_INIT;

size_t k_create_room(const struct k_room_config *config);

size_t k_get_room_idx_by_name(const char *name);

struct k_room_config {

    const char *name;

    int steps_per_second;

    void (*fn_create_event)(const struct k_room_context *room);
    void (*fn_destroy_event)(const struct k_room_context *room);
    int (*fn_entry_event)(const struct k_room_context *room);
    void (*fn_leave_event)(const struct k_room_context *room);
    void (*fn_step_event)(const struct k_room_context *room);
    void (*fn_draw_event)(const struct k_room_context *room);
};

extern const struct k_room_config K_ROOM_CONFIG_INIT;

struct k_room_context {

    uint32_t current_time;
    int delta_ms;
};

void k_goto_room(size_t room_idx);

#endif
