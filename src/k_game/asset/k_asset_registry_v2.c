#if 0

#include "./k_v2_asset_registry.h"

int k__asset_registry_init(struct k_asset_registry *registry, void (*fn_free_asset)(struct k_asset_registry_node *registry_node)) {

    size_t buckets_num = sizeof(registry->name_map_bucket) / sizeof(registry->name_map_bucket[0]);
    k_str_map_init(&registry->name_map, registry->name_map_bucket, buckets_num);

    k_hash_list_init(&registry->unnamed_list);

    registry->fn_free_asset = fn_free_asset;
    return 0;
}

static inline void free_assets_from_list(struct k_hash_list *list, void (*fn_free_asset)(struct k_asset_registry_node *)) {

    struct k_asset_registry_node *registry_node;
    struct k_str_map_node *map_node;
    struct k_hash_list_node *iter, *next;
    for(k_hash_list_for_each_s(list, iter, next)) {
        map_node = container_of(iter, struct k_str_map_node, list_node);
        registry_node = container_of(map_node, struct k_asset_registry_node, map_node);

        fn_free_asset(registry_node);
    }
}

void k__asset_registry_cleanup(struct k_asset_registry *registry) {

    void (*fn_free_asset)(struct k_asset_registry_node *) = registry->fn_free_asset;

    struct k_str_map *name_map = &registry->name_map;
    size_t i = 0;
    for (; i < registry->name_map.buckets_num; ++i) {
        free_assets_from_list(&name_map->buckets[i], fn_free_asset);
    }

    free_assets_from_list(&registry->unnamed_list, fn_free_asset);
}

int k__asset_registry_add(struct k_asset_registry *registry, struct k_asset_registry_node *registry_node, const char *asset_name) {

    struct k_str_map_node *map_node = &registry_node->map_node;

    if (NULL == asset_name || '\0' == asset_name[0]) {
        map_node->key = "";
        k_hash_list_add(&registry->unnamed_list, &map_node->list_node);
        return 0;
    }

    if (0 != k_str_map_add_if_absent(&registry->name_map, asset_name, map_node)) {
        // k_log_error("Failed to add asset: asset named \"%s\" already exists", asset_name);
        return -1;
    }

    return 0;
}

void k__asset_registry_del(struct k_asset_registry_node *registry_node) {

    if ('\0' != registry_node->map_node.key[0])
        k_str_map_del(&registry_node->map_node);

    k_hash_list_del(&registry_node->map_node.list_node);
}

struct k_asset_registry_node *k__asset_registry_find(struct k_asset_registry *registry, const char *asset_name) {

    if (NULL == asset_name || '\0' == asset_name[0])
        return NULL;

    struct k_str_map_node *map_node = k_str_map_get(&registry->name_map, asset_name);
    if (NULL == map_node)
        return NULL;

    struct k_asset_registry_node *registry_node = container_of(map_node, struct k_asset_registry_node, map_node);
    return registry_node;
}

const char *k__asset_get_name(struct k_asset_registry_node *registry_node) {
    return registry_node->map_node.key;
}

#endif
