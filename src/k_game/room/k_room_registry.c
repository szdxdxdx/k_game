#include "k_log.h"

#include "./k_room_registry.h"
#include "./k_room_create.h"
#include "./k_room_context.h"

static struct k_room_registry room_registry;

void k__room_registry_init(void) {
    k_list_init(&room_registry.rooms_list);

    static struct k_hash_list buckets[8]; /* <- 使用 8 个哈希桶应该足够，且不需动态扩容 */
    k_str_map_init(&room_registry.name_map, buckets, 8);
}

void k__room_registry_deinit(void) {

    struct k_list_node *iter, *next;
    for(k_list_for_each_s(&room_registry.rooms_list, iter, next)) {
        struct k_room_registry_node *room_node = container_of(iter, struct k_room_registry_node, iter_node);
        struct k_room *room = container_of(room_node, struct k_room, room_node);

        k__destroy_room(room);
    }
}

int k__room_registry_add(struct k_room_registry_node *node, const char *room_name) {

    if (NULL == room_name || '\0' == room_name[0]) {
        node->name_map_node.key = "";
    }
    else {
        if (0 != k_str_map_add(&room_registry.name_map, room_name, &node->name_map_node)) {
            k_log_error("The room named \"%s\" already exists", room_name);
            return -1;
        }
    }

    k_list_add_tail(&room_registry.rooms_list, &node->iter_node);
    return 0;
}

void k__room_registry_del(struct k_room_registry_node *node) {

    if ('\0' != node->name_map_node.key[0])
        k_str_map_del(&room_registry.name_map, &node->name_map_node);

    k_list_del(&node->iter_node);
}

struct k_room *k_get_room_by_name(const char *room_name) {

    if (NULL == room_name || '\0' == room_name[0])
        return NULL;

    struct k_str_map_node *map_node = k_str_map_get(&room_registry.name_map, room_name);
    if (NULL == map_node)
        return NULL;

    struct k_room_registry_node *room_node = container_of(map_node, struct k_room_registry_node, name_map_node);
    struct k_room *room = container_of(room_node, struct k_room, room_node);
    return room;
}

const char *k_room_get_name(struct k_room *room) {
    return room->room_node.name_map_node.key;
}
