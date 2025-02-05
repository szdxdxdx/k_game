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

/* region [room_create] */

struct k__room_registry_node {
    struct k_list_node iter_node;
    struct k__room *room;
};

struct k__room_registry {
    size_t room_id_counter;
    struct k_list rooms_iter_list;
};

int k__init_room_registry(const struct k_game_config *config);

void k__deinit_room_registry(void);

struct k__room {

    size_t id;
    const char *name;

    int (*fn_create_event)(const struct k_room *room);
    void (*fn_destroy_event)(const struct k_room *room);
    int (*fn_entry_event)(const struct k_room *room);
    void (*fn_leave_event)(const struct k_room *room);
    void (*fn_step_event)(const struct k_room *room);
    void (*fn_draw_event)(const struct k_room *room);

    Uint32 frame_interval;

    unsigned int game_loop;

    struct k_room room;
};

struct k__room *k__get_room(size_t room_id);

void k__run_room(struct k__room *room);

/* endregion */

/* region [room_goto] */

int k__init_room_stack(const struct k_game_config *config);

void k__deinit_room_stack(void);

int k_room_stack_push(struct k__room *room);

void k_room_stack_pop(void);

struct k__room *k_room_stack_get_top(void);

void k_room_stack_clear(void);

/* endregion */

#endif
