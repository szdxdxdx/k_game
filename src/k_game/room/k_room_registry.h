#ifndef K_GAME__ROOM_REGISTRY_H
#define K_GAME__ROOM_REGISTRY_H

#include "../asset/asset_registry.h"

struct k_room;

/* 房间注册表
 *
 */
struct k_room_registry {
    struct k_asset_registry registry;
};

/* 用于房间注册表的结点结构
 *
 */
struct k_room_registry_node {
    struct k_asset_registry_node registry_node;
};

int k__room_registry_init(void);

void k__room_registry_cleanup(void);

int k__room_registry_add(struct k_room *room, const char *room_name);

void k__room_registry_del(struct k_room *room);

#endif
