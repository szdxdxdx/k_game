#include <string.h>
#include <assert.h>

#include "k_game/core/k_mem_alloc.h"

#include "./k_asset_registry.h"

#define K_LOG_TAG "k_game:asset"
#include "k_log.h"

static char *k__asset_registry_strdup(const char *str) {

    char *copy = k__mem_alloc(strlen(str) + 1);
    if (NULL == copy)
        return NULL;

    strcpy(copy, str);
    return copy;
}

int k__asset_registry_init(struct k_asset_registry *registry) {

    size_t buckets_num = 32;
    struct k_hash_list *buckets = k__mem_alloc(buckets_num * sizeof(struct k_hash_list));
    if (NULL == buckets)
        return -1;

    k_str_intrusive_map_init(&registry->name_map, buckets, buckets_num);
    k_list_init(&registry->asset_list);
    return 0;
}

void k__asset_registry_cleanup(struct k_asset_registry *registry, void (*fn_release)(struct k_asset_registry_node *registry_node)) {
    assert(NULL != fn_release);

    struct k_asset_registry_node *registry_node;
    struct k_list *asset_list = &registry->asset_list;
    struct k_list_node *iter, *next;
    for (k_list_for_each_s(asset_list, iter, next)) {
        registry_node = container_of(iter, struct k_asset_registry_node, iter_node);

        /* fn_release() 中一定要调用 k__asset_registry_del() */
        fn_release(registry_node);
    }

    k__mem_free(registry->name_map.buckets);
}

void k__asset_registry_add(struct k_asset_registry *registry, struct k_asset_registry_node *registry_node) {
    registry_node->name_map_node.key = "";
    k_list_insert_tail(&registry->asset_list, &registry_node->iter_node);
}

void k__asset_registry_del(struct k_asset_registry_node *registry_node) {

    if ('\0' != registry_node->name_map_node.key[0]) {
        k_str_intrusive_map_remove(&registry_node->name_map_node);
        k_hash_list_node_loop(&registry_node->name_map_node.node_link);
    }

    k_list_remove(&registry_node->iter_node);
    k_list_node_loop(&registry_node->iter_node);
}

int k__asset_set_name(struct k_asset_registry *registry, struct k_asset_registry_node *registry_node, const char *asset_name) {

    struct k_str_intrusive_map_node *map_node = &registry_node->name_map_node;

    if (NULL == asset_name || '\0' == asset_name[0]) {

        if ('\0' != map_node->key[0]) {
            k_str_intrusive_map_remove(map_node);
            k__mem_free((void *)map_node->key);
            map_node->key = "";
        }

        return 0;
    }
    else {
        if (NULL != k_str_intrusive_map_get(&registry->name_map, asset_name)) {
            k_log_error("failed to set name: asset with name \"%s\" already exists");
            return -1;
        }

        char *name_copy = k__asset_registry_strdup(asset_name);
        if (NULL == name_copy)
            return -1;

        if ('\0' != map_node->key[0]) {
            k_str_intrusive_map_remove(map_node);
            k__mem_free((void *)map_node->key);
        }

        k_str_intrusive_map_add_directly(&registry->name_map, name_copy, map_node);
        return 0;
    }
}

const char *k__asset_get_name(struct k_asset_registry_node *registry_node) {
    return registry_node->name_map_node.key;
}

struct k_asset_registry_node *k__asset_registry_find(struct k_asset_registry *registry, const char *asset_name) {

    if (NULL == asset_name || '\0' == asset_name[0]) {
        k_log_warn("cannot find asset: `asset_name` is empty or null");
        return NULL;
    }

    struct k_str_intrusive_map_node *map_node = k_str_intrusive_map_get(&registry->name_map, asset_name);
    if (NULL == map_node)
        return NULL;

    struct k_asset_registry_node *registry_node = container_of(map_node, struct k_asset_registry_node, name_map_node);
    return registry_node;
}
