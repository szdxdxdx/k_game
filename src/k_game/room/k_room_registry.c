#include "./k_room_create.h"
#include "./k_room_context.h"
#include "./k_room_registry.h"

static struct k_asset_registry room_registry;

static void fn_free_asset(struct k_asset_registry_node *node) {
    struct k_room *room = container_of(node, struct k_room, registry_node);
    k__destroy_room(room);
}

int k__room_registry_init(void) {
    return k__asset_registry_init(&room_registry, fn_free_asset);
}

void k__room_registry_cleanup(void) {
     k__asset_registry_cleanup(&room_registry);
}

int k__room_registry_add(struct k_room *room, const char *room_name) {
    return k__asset_registry_add(&room_registry, &room->registry_node, room_name);
}

void k__room_registry_del(struct k_room *room) {
    k__asset_registry_del(&room->registry_node);
}

struct k_room *k_get_room_by_name(const char *room_name) {
    struct k_asset_registry_node *registry_node = k__asset_registry_get(&room_registry, room_name);
    if (NULL == registry_node)
        return NULL;

    struct k_room *room = container_of(registry_node, struct k_room, registry_node);
    return room;
}

const char *k_room_get_name(struct k_room *room) {
    return k__asset_get_name(&room->registry_node);
}
