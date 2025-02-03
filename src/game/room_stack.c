#include <assert.h>
#include "k/log.h"

#include "k/game.h"
#include "_internal_.h"

#ifndef K_ROOM_STACK_MAX_SIZE
#define K_ROOM_STACK_MAX_SIZE 32
#endif

struct room_stack {
    struct k_room *rooms[32];
    size_t top;
};

static struct room_stack room_stack = { .top = 0 };

int k_room_stack_push(struct k_room *room) {

    if (K_ROOM_STACK_MAX_SIZE == room_stack.top) {
        k_log_error("Failed to push room to stack. Room stack is full");
        return -1;
    }

    room_stack.rooms[room_stack.top] = room;
    room_stack.top += 1;

    return 0;
}

void k_room_stack_pop(void) {

    if (0 != room_stack.top)
        room_stack.top -= 1;
}

struct k_room *k_room_stack_get_top(void) {

    if (0 == room_stack.top)
        return NULL;
    else
        return room_stack.rooms[room_stack.top - 1];
}

struct k_room *k_room_stack_pop_top(void) {

    if (0 == room_stack.top)
        return NULL;

    struct k_room *top = room_stack.rooms[room_stack.top - 1];
    room_stack.top -= 1;

    return top;
}

void k_room_stack_clear(void) {
    room_stack.top = 0;
}

int k_goto_room(size_t room_idx) {

    struct k_room_registry_node *room_node = k_room_registry_get(room_idx);
    if (NULL == room_node) {
        k_log_error("Can not goto room with index %zu. "
                    "Failed to find this room", room_idx);
        return -1;
    }

    struct k_room *room = container_of(room_node, struct k_room, registry_node);
    if (0 != k_room_stack_push(room))
        return -1;

    return 0;
}
