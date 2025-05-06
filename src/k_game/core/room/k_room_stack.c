#include <stddef.h>

#define K_LOG_TAG "k_game:room"
#include "k_log.h"

#include "./k_room_stack.h"

/* region [room_stack] */

#ifndef K_ROOM_STACK_MAX_SIZE
#define K_ROOM_STACK_MAX_SIZE 32
#endif

struct k_room_stack {

    struct k_room *rooms[K_ROOM_STACK_MAX_SIZE];

    size_t top;
};

static struct k_room_stack room_stack;

static void room_stack_clear(void) {
    room_stack.top = 0;
}

static int room_stack_is_full(void) {
    return K_ROOM_STACK_MAX_SIZE == room_stack.top;
}

static int room_stack_is_empty(void) {
    return 0 == room_stack.top;
}

static void room_stack_push(struct k_room *room) {
    room_stack.rooms[room_stack.top++] = room;
}

static void room_stack_pop(void) {
    room_stack.top--;
}

static struct k_room *room_stack_get_top(void) {
    return room_stack.rooms[room_stack.top - 1];
}

void k__init_room_stack(void) {
    room_stack_clear();
}

void k__cleanup_room_stack(void) {
    room_stack_clear();
}

/* endregion */

/* region [room_goto] */

struct k_room *k__room_stack_get_top(void) {
    return room_stack_is_empty() ? NULL : room_stack_get_top();
}

int k_room_goto(struct k_room *room) {

    if (NULL == room) {
        k_log_error("room is null");
        return -1;
    }

    if (room_stack_is_full()) {
        k_log_error("room stack is full");
        return -1;
    }

    room_stack_push(room);

    return 0;
}

/* endregion */
