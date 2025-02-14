#include <string.h>

#include "k_log.h"
#include "k_container_of.h"

#include "./k_room_registry.h"
#include "./k_room_create.h"
#include "./k_room_context.h"

struct k_room_registry {
    struct k_list rooms_list;
};

static struct k_room_registry registry;

void k_room_registry_init(void) {
    k_init_list(&registry.rooms_list);
}

void k_room_registry_deinit(void) {

    struct k_list_node *iter, *next;
    for(k_list_for_each_s(&registry.rooms_list, iter, next)) {
        struct k_room_registry_node *room_node = container_of(iter, struct k_room_registry_node, iter_node);
        struct k_room *room = container_of(room_node, struct k_room, room_node);

        k_destroy_room(room);
    }
}

static struct k_room *room_registry_find_by_name(const char *room_name) {

    struct k_list_node *iter;
    for(k_list_for_each(&registry.rooms_list, iter)) {
        struct k_room_registry_node *room_node = container_of(iter, struct k_room_registry_node, iter_node);

        if (0 == strcmp(room_node->room_name, room_name))
            return container_of(room_node, struct k_room, room_node);
    }

    return NULL;
}

int k_room_registry_add(struct k_room_registry_node *node) {

    const char *room_name = node->room_name;
    if (NULL != room_registry_find_by_name(room_name)) {
        k_log_error("The room named \"%s\" already exists", room_name);
        return -1;
    }

    k_list_add_tail(&registry.rooms_list, &node->iter_node);
    return 0;
}

void k_room_registry_del(struct k_room_registry_node *node) {
    k_list_del(&node->iter_node);
}
