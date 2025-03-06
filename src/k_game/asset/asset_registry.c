#include "k_log.h"

#include "k_game_alloc.h"
#include "./asset_registry.h"

int k__asset_registry_init(struct k_asset_registry *registry, void (*fn_free)(struct k_asset_registry_node *registry_node)) {

    size_t buckets_num = 31;
    struct k_hash_list *buckets = k_malloc(buckets_num * sizeof(struct k_hash_list));
    if (NULL == buckets)
        return -1;

    k_list_init(&registry->asset_list);
    k_str_map_init(&registry->name_map, buckets, buckets_num);
    registry->fn_free = fn_free;

    return 0;
}

void k__asset_registry_cleanup(struct k_asset_registry *registry) {

    void (*fn_free)(struct k_asset_registry_node *) = registry->fn_free;
    struct k_asset_registry_node *registry_node;

    struct k_list *asset_list = &registry->asset_list;
    struct k_list_node *iter, *next;
    for(k_list_for_each_s(asset_list, iter, next)) {
        registry_node = container_of(iter, struct k_asset_registry_node, iter_node);

        fn_free(registry_node);
    }

    k_free(registry->name_map.buckets);
}

int k__asset_registry_add(struct k_asset_registry *registry, struct k_asset_registry_node *registry_node, const char *asset_name) {

    if (NULL == asset_name || '\0' == asset_name[0]) {
        registry_node->name_map_node.key = "";
    } else {
        if (0 != k_str_map_add(&registry->name_map, asset_name, &registry_node->name_map_node)) {
            k_log_error("Failed to add asset: asset named \"%s\" already exists", asset_name);
            return -1;
        }
    }

    k_list_add_tail(&registry->asset_list, &registry_node->iter_node);
    return 0;
}

void k__asset_registry_del(struct k_asset_registry_node *registry_node) {

    if ('\0' != registry_node->name_map_node.key[0])
        k_str_map_del(&registry_node->name_map_node);

    k_list_del(&registry_node->iter_node);
}

struct k_asset_registry_node *k__asset_registry_find(struct k_asset_registry *registry, const char *asset_name) {

    if (NULL == asset_name || '\0' == asset_name[0])
        return NULL;

    struct k_str_map_node *map_node = k_str_map_get(&registry->name_map, asset_name);
    if (NULL == map_node)
        return NULL;

    struct k_asset_registry_node *registry_node = container_of(map_node, struct k_asset_registry_node, name_map_node);
    return registry_node;
}

const char *k__asset_get_name(struct k_asset_registry_node *registry_node) {
    return registry_node->name_map_node.key;
}
