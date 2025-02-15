#include "k_log.h"
#include "k_container_of.h"

#include "./k_room_registry.h"
#include "./k_room_create.h"
#include "./k_room_context.h"

struct k_room_registry {
    struct k_list rooms_list;
    struct k_str_map name_map;
};

static struct k_room_registry registry;

void k_room_registry_init(void) {

    k_list_init(&registry.rooms_list);

    static struct k_hash_list name_map_buckets[8];
    k_str_map_init(&registry.name_map, name_map_buckets, 8);
}

void k_room_registry_deinit(void) {

    struct k_list_node *iter, *next;
    for(k_list_for_each_s(&registry.rooms_list, iter, next)) {
        struct k_room_registry_node *room_node = container_of(iter, struct k_room_registry_node, iter_node);
        struct k_room *room = container_of(room_node, struct k_room, room_node);

        k_destroy_room(room);
    }
}

int k_room_registry_add(struct k_room_registry_node *node, const char *room_name) {

    if (NULL == room_name || '\0' == room_name[0]) {
        node->name_map_node.key = NULL;
    }
    else {
        if (NULL != k_str_map_get(&registry.name_map, room_name)) {
            k_log_error("The room named \"%s\" already exists", room_name);
            return -1;
        }

        k_str_map_add(&registry.name_map, room_name, &node->name_map_node);
    }

    k_list_add_tail(&registry.rooms_list, &node->iter_node);
    return 0;
}

void k_room_registry_del(struct k_room_registry_node *node) {

    if (NULL != node->name_map_node.key)
        k_str_map_del(&registry.name_map, &node->name_map_node);

    k_list_del(&node->iter_node);
}

struct k_room *k_get_room_by_name(const char *room_name) {

    struct k_str_map_node *map_node = k_str_map_get(&registry.name_map, room_name);
    if (NULL == map_node)
        return NULL;

    struct k_room_registry_node *room_node = container_of(map_node, struct k_room_registry_node, name_map_node);
    struct k_room *room = container_of(room_node, struct k_room, room_node);
    return room;
}
