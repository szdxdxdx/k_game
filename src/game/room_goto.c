#include "k/log.h"

#include "k/game.h"
#include "_internal_.h"

#ifndef K_ROOM_STACK_MAX_SIZE
#define K_ROOM_STACK_MAX_SIZE 32
#endif

/* region [room stack] */

struct k__room_stack {
    struct k_room *rooms[32];
    size_t top;
};

static struct k__room_stack room_stack;

static inline void room_stack_clear(void) {
    room_stack.top = 0;
}

static inline int room_stack_is_full(void) {
    return K_ROOM_STACK_MAX_SIZE == room_stack.top;
}

static inline int room_stack_is_empty(void) {
    return 0 == room_stack.top;
}

static inline void room_stack_push(struct k_room *room) {
    room_stack.rooms[room_stack.top++] = room;
}

static inline void room_stack_pop(void) {
    room_stack.top--;
}

static inline struct k_room *room_stack_get_top(void) {
    return room_stack.rooms[room_stack.top - 1];
}

/* endregion */

/* region [init] */

int k__init_room_stack(void) {
    room_stack_clear();
    return 0;
}

void k__deinit_room_stack(void) {
    room_stack_clear();
}

/* endregion */

/* region [room goto] */

struct k_room *k_room_stack_get_top(void) {
    return room_stack_is_empty() ? NULL : room_stack_get_top();
}

int k_goto_room(struct k_room *room) {

    if (NULL == room) {
        k_log_error("Failed to goto room. Room is NULL");
        return -1;
    }

    if (room_stack_is_full()) {
        k_log_error("Failed to goto room { .name=\"%s\" }. Room stack is full", room->name);
        return -1;
    }

    room_stack_push(room);

    k_log_trace("Next room set to { .name=\"%s\" }", room->name);
    return 0;
}

/* endregion */
