#include "./k_room_registry.h"
#include "./k_room_create.h"
#include "./k_room_entity.h"

#include "../game/k_game_context.h"

int k__room_registry_init(void) {
    return k__asset_registry_init(&k__game.room_registry);
}

static void release_asset(struct k_asset_registry_node *node) {
    struct k_room *room = container_of(node, struct k_room, registry_node);
    k_room__destroy(room);
}

void k__room_registry_cleanup(void) {
    k__asset_registry_cleanup(&k__game.room_registry, release_asset);
}

int k_room_set_name(struct k_room *room, const char *room_name) {
    return k__asset_set_name(&k__game.room_registry, &room->registry_node, room_name);
}

struct k_room *k_room_find(const char *room_name) {
    struct k_asset_registry_node *registry_node = k__asset_registry_find(&k__game.room_registry, room_name);
    if (NULL == registry_node)
        return NULL;

    struct k_room *room = container_of(registry_node, struct k_room, registry_node);
    return room;
}
