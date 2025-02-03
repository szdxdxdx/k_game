#include <limits.h>

#include "k/log.h"
#include "k/list.h"

#include "k/game.h"
#include "_internal_.h"

struct k_room_registry * const k_room_registry = &(struct k_room_registry){};

int k_init_room_registry(void) {

    k_room_registry->idx_counter = 0;
    k_init_list(&k_room_registry->iter_list);

    return 0;
}

void k_deinit_room_registry(void) {

    /* TODO */
}

static inline size_t get_new_idx(void) {
    return ++k_room_registry->idx_counter;
}

int k_room_registry_add(struct k_room_registry_node *node) {

    /* TODO 将 room create 合并到这里 */

    node->idx = get_new_idx();
    k_list_add_tail(&k_room_registry->iter_list, &node->iter_list_node);

    return 0;
}

void k_room_registry_del(size_t room_idx) {

    /* TODO */
}

struct k_room_registry_node *k_room_registry_get(size_t room_idx) {

    struct k_room_registry_node *room_node;
    struct k_list_node *list_iterator;
    for (k_list_for_each(&k_room_registry->iter_list, list_iterator)) {
        room_node = container_of(list_iterator, struct k_room_registry_node, iter_list_node);

        if (room_idx == room_node->idx)
            return room_node;
    }

    return NULL;
}

size_t k_find_room_by_name(const char *name) {

    return SIZE_MAX;
}
