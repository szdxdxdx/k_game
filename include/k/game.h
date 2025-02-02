#ifndef K_GAME_H
#define K_GAME_H

#include <stddef.h>

struct k_game_config;
struct k_room_config;

struct k_room_context;

int k_run_game(const struct k_game_config *config);

struct k_game_config {

    int (*fn_game_setup)(void);
};

size_t k_create_room(const struct k_room_config *config);

struct k_room_config {

    void (*fn_create_room)(const struct k_room_context *room);
    void (*fn_destroy_room)(const struct k_room_context *room);
    int (*fn_entry_room)(const struct k_room_context *room);
    void (*fn_leave_room)(const struct k_room_context *room);
    void (*fn_room_step)(const struct k_room_context *room);
    void (*fn_draw_room)(const struct k_room_context *room);
};

#endif
