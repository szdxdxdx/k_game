#include "k_game/core/k_mem_alloc.h"

#include "./k_asset_registry.h"

int k__asset_registry_init(struct k_asset_registry *registry) {

    size_t buckets_num = 32;
    struct k_hash_list *buckets = k_mem_alloc(buckets_num * sizeof(struct k_hash_list));
    if (NULL == buckets)
        return -1;

    k_str_hash_map_init(&registry->name_map, buckets, buckets_num);
    k_list_init(&registry->asset_list);
    return 0;
}

void k__asset_registry_cleanup(struct k_asset_registry *registry, void (*fn_release_asset)(struct k_asset_registry_node *registry_node)) {

    struct k_asset_registry_node *registry_node;
    struct k_list *asset_list = &registry->asset_list;
    struct k_list_node *iter, *next;
    for(k_list_for_each_s(asset_list, iter, next)) {
        registry_node = container_of(iter, struct k_asset_registry_node, iter_node);

        k__asset_registry_del(registry_node);
        fn_release_asset(registry_node);
    }

    k_mem_free(registry->name_map.buckets);
}

void k__asset_registry_add(struct k_asset_registry *registry, struct k_asset_registry_node *registry_node) {
    registry_node->name_map_node.key = "";
    k_list_add_tail(&registry->asset_list, &registry_node->iter_node);
}

void k__asset_registry_del(struct k_asset_registry_node *registry_node) {

    if ('\0' != registry_node->name_map_node.key[0]) {
        k_str_hash_map_del(&registry_node->name_map_node);
        k_hash_list_node_loop(&registry_node->name_map_node.list_node);
    }

    k_list_del(&registry_node->iter_node);
    k_list_node_loop(&registry_node->iter_node);
}

int k__asset_set_name(struct k_asset_registry *registry, struct k_asset_registry_node *registry_node, const char *asset_name) {

    struct k_str_hash_map_node *map_node = &registry_node->name_map_node;

    if (NULL == asset_name || '\0' == asset_name[0]) {

        if ('\0' != map_node->key[0]) {
            k_str_hash_map_del(map_node);
            map_node->key = "";
        }
    }
    else {
        if ('\0' != map_node->key[0])
            k_str_hash_map_del(map_node);

        if (0 != k_str_hash_map_add_if_absent(&registry->name_map, asset_name, map_node)) {
            return -1; /* TODO log("同名资源已存在") */
        }
    }

    return 0;
}

const char *k__asset_get_name(struct k_asset_registry_node *registry_node) {
    return registry_node->name_map_node.key;
}

struct k_asset_registry_node *k__asset_registry_find(struct k_asset_registry *registry, const char *asset_name) {

    if (NULL == asset_name || '\0' == asset_name[0])
        return NULL;

    struct k_str_hash_map_node *map_node = k_str_hash_map_get(&registry->name_map, asset_name);
    if (NULL == map_node)
        return NULL;

    struct k_asset_registry_node *registry_node = container_of(map_node, struct k_asset_registry_node, name_map_node);
    return registry_node;
}
