#include <assert.h>

#include "k_game/room.h"
#include "./k_room_context.h"

const char *k_room_get_name(struct k_room *room) {
    assert(NULL != room);
    return room->room_node.name_map_node.key;
}
