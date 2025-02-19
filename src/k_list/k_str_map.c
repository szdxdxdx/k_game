#include <string.h>

#include "k_str_map.h"

static inline size_t str_hash(const char *str) {

    size_t hash = 5381;
    for (; '\0' != *str; str++) {
        hash = hash * 33 + *str;
    }

    return hash;
}

static inline struct k_str_map_node *find(struct k_hash_list *list, const char *key, size_t hash) {

    struct k_hash_list_node *list_node;
    for (k_hash_list_for_each(list, list_node)) {

        struct k_str_map_node *map_node = container_of(list_node, struct k_str_map_node, list_node);

        if (map_node->hash == hash) {
            if (strcmp(map_node->key, key) == 0)
                return map_node;
        }
    }

    return NULL;
}

void k_str_map_init(struct k_str_map *map, struct k_hash_list *lists, size_t lists_num) {

    k_hash_list_init_all(lists, lists_num);

    map->lists = lists;
    map->lists_num = lists_num;
}

int k_str_map_add(struct k_str_map *map, const char *key, struct k_str_map_node *node) {

    size_t hash = str_hash(key);

    struct k_hash_list *list = &(map->lists[hash % map->lists_num]);
    if (NULL != find(list, key, hash))
        return -1;

    node->key = key;
    node->hash = hash;
    k_hash_list_add(list, &node->list_node);

    return 0;
}

struct k_str_map_node *k_str_map_get(struct k_str_map *map, const char *key) {
    size_t hash = str_hash(key);
    struct k_hash_list *list = &(map->lists[hash % map->lists_num]);
    return find(list, key, hash);
}

void k_str_map_del(struct k_str_map *map, struct k_str_map_node *node) {
    k_hash_list_del(&node->list_node);
}

struct k_hash_list *k_str_map_rehash(struct k_str_map *map, struct k_hash_list *new_lists, size_t new_lists_num) {

    k_hash_list_init_all(new_lists, new_lists_num);

    struct k_hash_list *old_lists = map->lists;
    size_t old_lists_num = map->lists_num;

    struct k_hash_list *old_list = old_lists;
    for (; old_list < old_lists + old_lists_num; old_list++) {

        struct k_hash_list_node *list_node, *next;
        for (k_hash_list_for_each_s(old_list, list_node, next)) {

            struct k_str_map_node *map_node = container_of(list_node, struct k_str_map_node, list_node);
            struct k_hash_list *new_list = &(new_lists[map_node->hash % new_lists_num]);
            k_hash_list_add(new_list, &map_node->list_node);
        }
    }

    map->lists = new_lists;
    map->lists_num  = new_lists_num;

    return old_lists;
}
