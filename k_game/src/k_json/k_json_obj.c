#include <string.h>

#include "./_internal.h"

static inline size_t calc_str_hash(const char *str, size_t len) {

    size_t hash = 1;
    size_t idx = 0;
    for (; idx < len; idx++)
        hash = ((hash << 5) + hash) + str[idx];
    return hash;
}

struct k_json_obj *k__json_obj_create(void) {

    struct k_json_obj *json_obj = NULL;
    struct k_hash_list *lists = NULL;

    size_t lists_num = 7;
    json_obj = k__json_mem_alloc(sizeof(struct k_json_obj));
    lists    = k__json_mem_alloc(sizeof(struct k_hash_list) * lists_num);
    if (json_obj == NULL || lists == NULL)
        goto err;

    k_hash_list_init_all(lists, lists_num);

    json_obj->json.type = K__JSON_OBJ;
    json_obj->lists     = lists;
    json_obj->lists_num = lists_num;
    return json_obj;

err:
    k__json_mem_free(lists);
    k__json_mem_free(json_obj);
    return NULL;
}

void k__json_obj_destroy(struct k_json_obj *json_obj) {

    size_t i = 0;
    for (; i < json_obj->lists_num; i++) {

        struct k_hash_list_node *node, *next;
        for (k_hash_list_for_each_s(&(json_obj->lists[i]), node, next)) {
            struct k_json_obj_pair *pair = container_of(node, struct k_json_obj_pair, link);

            k__json_destroy(pair->val);
            k__json_mem_free(pair->key);
            k__json_mem_free(pair);
        }
    }

    k__json_mem_free(json_obj->lists);
    k__json_mem_free(json_obj);
}

int k__json_obj_add(struct k_json_obj *json_obj, const char *key, size_t key_len, struct k_json *val) {

    size_t key_hash = calc_str_hash(key, key_len);

    struct k_hash_list *list = &json_obj->lists[key_hash % json_obj->lists_num];
    struct k_hash_list_node *node;
    for (k_hash_list_for_each(list, node)) {
        struct k_json_obj_pair *pair = container_of(node, struct k_json_obj_pair, link);

        if (pair->key_hash == key_hash && 0 == strcmp(pair->key, key)) {
            k__json_destroy(pair->val);
            pair->val = val;
            return 0;
        }
    }

    char *key_copy = NULL;
    struct k_json_obj_pair *new_pair = NULL;
    key_copy = k__json_strdup(key, key_len);
    new_pair = k__json_mem_alloc(sizeof(struct k_json_obj_pair));
    if (key_copy == NULL || new_pair == NULL)
        goto err;

    k_hash_list_add(&json_obj->lists[key_hash % json_obj->lists_num], &new_pair->link);
    new_pair->key      = key_copy;
    new_pair->key_hash = key_hash;
    new_pair->val      = val;

    return 0;

err:
    k__json_mem_free(key_copy);
    k__json_mem_free(new_pair);
    return -1;
}

struct k_json_obj_pair *k__json_obj_get(struct k_json_obj *json_obj, const char *key, size_t key_len) {

    size_t key_hash = calc_str_hash(key, key_len);

    struct k_hash_list *list = &json_obj->lists[key_hash % json_obj->lists_num];
    struct k_hash_list_node *node;
    for (k_hash_list_for_each(list, node)) {

        struct k_json_obj_pair *pair = container_of(node, struct k_json_obj_pair, link);
        if (pair->key_hash == key_hash && 0 == strcmp(pair->key, key))
            return pair;
    }

    return NULL;
}

void k__json_obj_del(struct k_json_obj *json_obj, const char *key) {

    struct k_json_obj_pair *pair = k__json_obj_get(json_obj, key, strlen(key));
    if (NULL == pair)
        return;

    k_hash_list_remove(&pair->link);
    k__json_destroy(pair->val);
    k__json_mem_free(pair->key);
    k__json_mem_free(pair);
}
