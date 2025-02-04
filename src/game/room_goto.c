#include "k/log.h"

#include "k/game.h"
#include "_internal_.h"

#ifndef K_ROOM_STACK_MAX_SIZE
#define K_ROOM_STACK_MAX_SIZE 32
#endif

struct room_stack {
    struct k__room *rooms[32];
    size_t top;
};

static struct room_stack room_stack;

int k__init_room_stack(const struct k_game_config *config) {
    (void)config;

    room_stack.top = 0;

    k_log_trace("Room stack initialized");
    return 0;
}

void k__deinit_stack(void) {
    room_stack.top = 0;

    k_log_trace("Room stack deinitialized");
}

int k_room_stack_push(struct k__room *room) {

    if (K_ROOM_STACK_MAX_SIZE == room_stack.top) {
        k_log_error("Failed to push room to stack. Room stack is full");
        return -1;
    }

    room_stack.rooms[room_stack.top] = room;
    room_stack.top += 1;

    k_log_trace("Pushed room { .name=\"%s\", .id=%zu } to stack", room->name, room->id);
    return 0;
}

void k_room_stack_pop(void) {

    if (0 != room_stack.top)
        room_stack.top -= 1;
}

struct k__room *k_room_stack_get_top(void) {

    if (0 == room_stack.top)
        return NULL;
    else
        return room_stack.rooms[room_stack.top - 1];
}

struct k__room *k_room_stack_pop_top(void) {

    if (0 == room_stack.top)
        return NULL;

    struct k__room *top = room_stack.rooms[room_stack.top - 1];
    room_stack.top -= 1;

    return top;
}

void k_room_stack_clear(void) {
    room_stack.top = 0;
}

int k_goto_room(size_t room_idx) {

    struct k__room *room = k__get_room(room_idx);
    if (NULL == room)
        goto err;

    if (0 != k_room_stack_push(room))
        goto err;

    return 0;

err:
    k_log_error("Failed to goto room { .id=%zu }", room_idx);
    return -1;
}
