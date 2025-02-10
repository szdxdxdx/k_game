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

typedef struct k__game_context k__game_context;

struct k__game_context {

    SDL_Window *window;

    SDL_Renderer *renderer;

    unsigned int quit_game;

    struct k_room *current_room;
};

extern struct k__game_context k__game;

/* endregion */

/* region [room_create] */

int k__init_room_registry(void);

void k__deinit_room_registry(void);

struct k__room_node {
    struct k_list_node iter_node;
};

struct k_room {

    struct k__room_node room_node;

    const char *name;

    int  (*fn_create) (const struct k_room *room);
    void (*fn_destroy)(const struct k_room *room);
    int  (*fn_enter)  (const struct k_room *room);
    void (*fn_leave)  (const struct k_room *room);
    void (*fn_step)   (const struct k_room *room);
    void (*fn_draw)   (const struct k_room *room);

    Uint32 frame_interval;

    unsigned int game_loop;

    uint64_t current_time;

    int delta_ms;

    void *data;
};

struct k_room *k__get_room(size_t room_id);

void k__run_room(struct k_room *room);

/* endregion */

/* region [room_goto] */

int k__init_room_stack(void);

void k__deinit_room_stack(void);

int k_room_stack_push(struct k_room *room);

void k_room_stack_pop(void);

struct k_room *k_room_stack_get_top(void);

void k_room_stack_clear(void);

/* endregion */

#endif
