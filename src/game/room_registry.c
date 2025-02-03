#include <limits.h>

#include "k/log.h"
#include "k/list.h"

#include "k/game.h"
#include "_internal_.h"

struct k_room_registry * const k_room_registry = &(struct k_room_registry){};

int k_init_room_registry(void) {

    k_init_list(&k_room_registry->iter_list);
    return 0;
}

void k_deinit_room_registry(void) {

    /* TODO */
}

size_t k_room_registry_add(struct k_room *room) {

    return 0;
}

void k_room_registry_del(size_t room_idx) {

    /* TODO */
}
