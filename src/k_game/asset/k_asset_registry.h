#ifndef K_GAME__ASSET_REGISTRY_H
#define K_GAME__ASSET_REGISTRY_H

#include "k_list.h"
#include "k_str_map.h"

struct k_asset_registry_node;

/* 资源注册表
 *
 * TODO docs
 */
struct k_asset_registry {

    struct k_str_map name_map;

    struct k_list asset_list;

    void (*fn_free_asset)(struct k_asset_registry_node *node);
};

/* 用于资源注册表的结点结构
 *
 * TODO docs
 */
struct k_asset_registry_node {

    struct k_str_map_node name_map_node;

    struct k_list_node iter_node;
};

int k__asset_registry_init(struct k_asset_registry *registry, void (*fn_free_asset)(struct k_asset_registry_node *registry_node));

void k__asset_registry_cleanup(struct k_asset_registry *registry);

void k__asset_registry_add(struct k_asset_registry *registry, struct k_asset_registry_node *registry_node);

void k__asset_registry_del(struct k_asset_registry_node *registry_node);

int k__asset_set_name(struct k_asset_registry *registry, struct k_asset_registry_node *registry_node, const char *name);

const char *k__asset_get_name(struct k_asset_registry_node *registry_node);

struct k_asset_registry_node *k__asset_registry_find(struct k_asset_registry *registry, const char *asset_name);

#endif
