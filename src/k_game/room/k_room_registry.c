#include "./k_room.h"

static struct k_asset_registry room_registry;

int k__room_registry_init(void) {
    return k__asset_registry_init(&room_registry);
}

static void fn_release_asset(struct k_asset_registry_node *node) {
    struct k_room *room = container_of(node, struct k_room, registry_node);
    k__room_destroy(room);
}

void k__room_registry_cleanup(void) {
     k__asset_registry_cleanup(&room_registry, fn_release_asset);
}

void k__room_registry_add(struct k_room *room) {
    k__asset_registry_add(&room_registry, &room->registry_node);
}

void k__room_registry_del(struct k_room *room) {
    k__asset_registry_del(&room->registry_node);
}

int k_room_set_name(struct k_room *room, const char *room_name) {
    return k__asset_set_name(&room_registry, &room->registry_node, room_name);
}

struct k_room *k_room_find(const char *room_name) {
    struct k_asset_registry_node *registry_node = k__asset_registry_find(&room_registry, room_name);
    if (NULL == registry_node)
        return NULL;

    struct k_room *room = container_of(registry_node, struct k_room, registry_node);
    return room;
}
