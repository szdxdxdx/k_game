#include <stddef.h>

#define K_LOG_TAG "k_game:room"
#include "k_log.h"

#include "./k_room_stack.h"
#include "./k_room.h"

#ifndef K_ROOM_STACK_MAX_SIZE
#define K_ROOM_STACK_MAX_SIZE 32
#endif

struct k_room_stack {

    /* 当前不允许主动销毁房间，所以栈中的指针不会悬空。
     * 若允许销毁房间，则栈应改为存储房间 ID */
    struct k_room *rooms[K_ROOM_STACK_MAX_SIZE];

    size_t top;
};

static struct k_room_stack room_stack;

static int k__room_stack_is_full(void) {
    return K_ROOM_STACK_MAX_SIZE == room_stack.top;
}

static int k__room_stack_is_empty(void) {
    return 0 == room_stack.top;
}

static void k__room_stack_push(struct k_room *room) {
    room_stack.rooms[room_stack.top++] = room;
}

static void k__room_stack_pop(void) {
    room_stack.top--;
}

void k__room_stack_init(void) {
    room_stack.top = 0;
}

void k__room_stack_clear(void) {
    room_stack.top = 0;
}

struct k_room *k__room_stack_get_top(void) {
    return k__room_stack_is_empty() ? NULL : room_stack.rooms[room_stack.top - 1];
}

void k_room_nav_goto(struct k_room *room) {

    if (NULL == room) {
        k_log_error("room is null");
        return;
    }

    if ( ! k__room_stack_is_empty()) {
        k__room_stack_pop();
    }
    k__room_stack_push(room);

    if (NULL != k__room_current) {
        k__room_current->game_loop = 0;
    }
}

void k_room_nav_push(struct k_room *room) {

    if (NULL == room) {
        k_log_error("room is null");
        return;
    }
    else if (k__room_stack_is_full()) {
        k_log_error("room stack is full");
        return;
    }

    k__room_stack_push(room);

    if (NULL != k__room_current) {
        k__room_current->game_loop = 0;
    }
}

void k_room_nav_pop(void) {
    if ( ! k__room_stack_is_empty()) {
        k__room_stack_pop();
    }

    if (NULL != k__room_current) {
        k__room_current->game_loop = 0;
    }
}
