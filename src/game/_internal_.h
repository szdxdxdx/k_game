#ifndef K_GAME_INTERNAL_H
#define K_GAME_INTERNAL_H

#include "SDL.h"

#include "k/list.h"

#ifndef container_of
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))
#endif

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

/* region [room_registry] */

struct k_room;

struct k_room_registry {

    size_t idx_counter;

    struct k_list iter_list;
};

struct k_room_registry_node {

    struct k_list_node iter_list_node;

    const char *name;

    size_t idx;
};

extern struct k_room_registry * const k_room_registry;

int k_init_room_registry(void);

void k_deinit_room_registry(void);

int k_room_registry_add(struct k_room_registry_node *node);

void k_room_registry_del(size_t room_idx);

struct k_room_registry_node *k_room_registry_get(size_t room_idx);

/* endregion */

/* region [room] */

struct k_room {

    struct k_room_registry_node registry_node;

    void (*fn_destroy_event)(const struct k_room_context *room);
    int (*fn_entry_event)(const struct k_room_context *room);
    void (*fn_leave_event)(const struct k_room_context *room);
    void (*fn_step_event)(const struct k_room_context *room);
    void (*fn_draw_event)(const struct k_room_context *room);

    Uint32 frame_interval;

    unsigned int game_loop;

    struct k_room_context ctx;
};

void k_run_room(struct k_room *room);

/* endregion */

/* region [room_stack] */

int k_room_stack_push(struct k_room *room);

void k_room_stack_pop(void);

struct k_room *k_room_stack_get_top(void);

void k_room_stack_clear(void);

/* endregion */

#endif
