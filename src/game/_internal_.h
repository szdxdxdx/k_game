#ifndef K_GAME_INTERNAL_H
#define K_GAME_INTERNAL_H

#include "SDL.h"

#include "k/list.h"

/* region [game] */

struct k_game_context {
    unsigned int quit_game;
};

extern struct k_game_context * const k_game;

/* endregion */

/* region [window] */

struct k_game_window {

    SDL_Window *window;
    int window_w;
    int window_h;

    SDL_Renderer *renderer;
};

extern struct k_game_window * const k_window;

/* endregion */

/* region [room] */

struct k_room_registry {

    struct k_list iter_list;
};

extern struct k_room_registry * const k_room_registry;

int k_init_room_registry(void);

void k_deinit_room_registry(void);

struct k_room_context;

struct k_room {

    const char *name;

    void (*fn_destroy_event)(const struct k_room_context *room);
    int (*fn_entry_event)(const struct k_room_context *room);
    void (*fn_leave_event)(const struct k_room_context *room);
    void (*fn_step_event)(const struct k_room_context *room);
    void (*fn_draw_event)(const struct k_room_context *room);

    Uint32 frame_interval;

    unsigned int game_loop;

    struct k_room_context ctx;
};

size_t k_room_registry_add(struct k_room *room);

/* endregion */

#endif
