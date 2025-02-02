#include <limits.h>

#include "k/log.h"
#include "k/list.h"

#include "k/game.h"
#include "k/internal/room_registry.h"
#include "k/internal/room_context.h"

struct k_room_registry * const k_room_registry = &(struct k_room_registry){};

int k_init_room_registry(void) {

    k_init_list(&k_room_registry->iter_list);

    return 0;
}

void k_deinit_room_registry(void) {

    /* TODO */
}

int k_room_registry_add(struct k_room_context *room) {

    return 0;
}
