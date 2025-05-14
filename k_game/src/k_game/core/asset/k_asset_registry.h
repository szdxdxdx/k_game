#ifndef K__ASSET_REGISTRY_H
#define K__ASSET_REGISTRY_H

#include "k_list.h"
#include "k_str_intrusive_map.h"

struct k_asset_registry {

    /* 用链表存储注册表中的所有资源 */
    struct k_list asset_list;

    /* 有名字的资源用哈希表建立映射 */
    struct k_str_intrusive_map name_map;
};

struct k_asset_registry_node {

    struct k_str_intrusive_map_node name_map_node;

    struct k_list_node iter_node;
};

int k__asset_registry_init(struct k_asset_registry *registry);

void k__asset_registry_cleanup(struct k_asset_registry *registry, void (*fn_release)(struct k_asset_registry_node *registry_node));

void k__asset_registry_add(struct k_asset_registry *registry, struct k_asset_registry_node *registry_node);

void k__asset_registry_del(struct k_asset_registry_node *registry_node);

/* 设置或修改资源的名字
 *
 * 若设置名字为空字符串或 "" 或 `NULL`，则清除资源名字，否则修改资源名字。
 * 若当前没有名字相同的资源，则修改成功，函数返回 0，否则返回非 0。
 */
int k__asset_set_name(struct k_asset_registry *registry, struct k_asset_registry_node *registry_node, const char *asset_name);

const char *k__asset_get_name(struct k_asset_registry_node *registry_node);

struct k_asset_registry_node *k__asset_registry_find(struct k_asset_registry *registry, const char *asset_name);

#endif
