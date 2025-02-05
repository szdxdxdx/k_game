#include "k/log.h"

#include "k/game.h"
#include "_internal_.h"

#ifndef K_ROOM_STACK_MAX_SIZE
#define K_ROOM_STACK_MAX_SIZE 32
#endif

/* region [room stack] */

struct room_stack {
    struct k__room *rooms[32];
    size_t top;
};

static struct room_stack room_stack;

static inline void room_stack_clear(void) {
    room_stack.top = 0;
}

static inline int room_stack_is_full(void) {
    return K_ROOM_STACK_MAX_SIZE == room_stack.top;
}

static inline int room_stack_is_empty(void) {
    return 0 == room_stack.top;
}

static inline void room_stack_push(struct k__room *room) {
    room_stack.rooms[room_stack.top++] = room;
}

static inline void room_stack_pop(void) {
    room_stack.top--;
}

static inline struct k__room *room_stack_get_top(void) {
    return room_stack.rooms[room_stack.top - 1];
}

/* endregion */

/* region [init] */

static int is_initialized = 0;

int k__init_room_stack(const struct k_game_config *config) {
    (void)config;

    if (is_initialized) {
        k_log_error("Room stack is already initialized");
        return -1;
    }

    room_stack_clear();
    is_initialized = 1;

    k_log_trace("Room stack initialized");
    return 0;
}

void k__deinit_room_stack(void) {

    if ( ! is_initialized)
        return;

    room_stack_clear();
    is_initialized = 0;

    k_log_trace("Room stack deinitialized");
}

/* endregion */

/* region [room goto] */

struct k__room *k_room_stack_get_top(void) {
    return room_stack_is_empty() ? NULL : room_stack_get_top();
}

int k_goto_room(size_t room_idx) {

    struct k__room *room = k__get_room(room_idx);
    if (NULL == room)
        goto err;

    if (room_stack_is_full()) {
        k_log_error("Failed to goto room { .id=%zu }. Room stack is full", room_idx);
        goto err;
    }

    room_stack_push(room);

    k_log_trace("Next room set to { .name=\"%s\", .id=%zu }", room->name, room->id);
    return 0;

err:
    k_log_error("Failed to goto room { .id=%zu }", room_idx);
    return -1;
}

/* endregion */
