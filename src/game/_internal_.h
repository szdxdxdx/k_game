#ifndef K_GAME_INTERNAL_H
#define K_GAME_INTERNAL_H

#include "SDL.h"

#include "k/list.h"

#ifndef container_of
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))
#endif

/* region [SDL] */

int k__init_SDL(const struct k_game_config *config);

void k__deinit_SDL(void);

/* endregion */

/* region [game] */

struct k__game_context {

    SDL_Window *window;

    SDL_Renderer *renderer;

    unsigned int quit_game;

    struct k__room *current_room;
};

extern struct k__game_context * const k__game;

/* endregion */

/* region [room] */

int k__init_room_registry(const struct k_game_config *config);

void k__deinit_room_registry(void);

struct k__room {

    size_t id;
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

struct k__room *k__get_room(size_t room_id);

void k__run_room(struct k__room *room);

/* endregion */

/* region [room_stack] */

int k_room_stack_push(struct k__room *room);

void k_room_stack_pop(void);

struct k__room *k_room_stack_get_top(void);

void k_room_stack_clear(void);

/* endregion */

#endif
