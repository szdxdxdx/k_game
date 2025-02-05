#ifndef K_GAME_H
#define K_GAME_H

#include <stddef.h>
#include <stdint.h>

/* region [game] */

struct k_game_config;

int k_run_game(const struct k_game_config *config);

struct k_game_config {

    int window_w;
    int window_h;

    int (*fn_setup_game)(void);
};

extern const struct k_game_config K_GAME_CONFIG_INIT;

/* endregion */

/* region [room] */

struct k_room_config;
struct k_room;

size_t k_create_room(const struct k_room_config *config);

void k_destroy_room(size_t room_id);

struct k_room_config {

    const char *room_name;

    size_t data_size;

    int steps_per_second;

    int (*fn_create_event)(const struct k_room *room);
    void (*fn_destroy_event)(const struct k_room *room);
    int (*fn_entry_event)(const struct k_room *room);
    void (*fn_leave_event)(const struct k_room *room);
    void (*fn_step_event)(const struct k_room *room);
    void (*fn_draw_event)(const struct k_room *room);
};

extern const struct k_room_config K_ROOM_CONFIG_INIT;

struct k_room {

    uint32_t current_time;

    int delta_ms;

    void *data;
};

int k_goto_room(size_t room_idx);

/* endregion */

#endif
