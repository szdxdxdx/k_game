#include <stdarg.h>
#include <string.h>
#include <assert.h>

#include "./_internal.h"

void k__json_destroy(struct k_json *json) {
    switch (json->type) {
        case K__JSON_NULL:
            k__json_null_destroy((struct k_json_null *)json); break;
        case K__JSON_BOOL:
            k__json_bool_destroy((struct k_json_bool *)json); break;
        case K__JSON_STR:
            k__json_destroy_str((struct k_json_str *)json); break;
        case K__JSON_NUM:
            k__json_num_destroy((struct k_json_num *)json); break;
        case K__JSON_ARR:
            k__json_arr_destroy((struct k_json_arr *)json); break;
        case K__JSON_OBJ:
            k__json_obj_destroy((struct k_json_obj *)json); break;
        default: break;
    }
}

struct k_json *k_json_parse(const char *fmt, ...) {

    if (NULL == fmt || '\0' == *fmt)
        return NULL;

    va_list args;
    va_start(args, fmt);
    struct k_json *json = k__json_parse(fmt, args);
    va_end(args);

    return json;
}

void k_json_free(struct k_json *json) {
    if (NULL != json)
        k__json_destroy(json);
}

int k_json_arr_add(struct k_json *json, size_t idx, struct k_json *val) {

    if (NULL == json || K__JSON_ARR != json->type)
        return -1;

    if (NULL == val)
        return -1;

    if (0 != k__json_arr_add((struct k_json_arr *)json, idx, val))
        return -1;

    return 0;
}

int k_json_obj_add(struct k_json *json, const char *key, struct k_json *val) {

    if (NULL == json || K__JSON_OBJ != json->type)
        return -1;

    if (NULL == key || NULL == val)
        return -1;

    if (0 != k__json_obj_add((struct k_json_obj *)json, key, strlen(key), val))
        return -1;

    return 0;
}

int k_json_arr_push(struct k_json *json, struct k_json *val) {

    if (NULL == json || K__JSON_ARR != json->type)
        return -1;

    if (NULL == val)
        return -1;

    struct k_json_arr *json_arr = (struct k_json_arr *)json;
    if (0 != k__json_arr_push(json_arr, val))
        return -1;

    return 0;
}

struct k_json *k_json_arr_get(struct k_json *json, size_t idx) {

    if (NULL == json || K__JSON_ARR != json->type)
        return NULL;

    return k__json_arr_get((struct k_json_arr *)json, idx);
}

struct k_json *k_json_obj_get(struct k_json *json, const char *key) {

    if (NULL == json || K__JSON_OBJ != json->type)
        return NULL;

    if (NULL == key)
        return NULL;

    struct k_json_obj_pair *pair = k__json_obj_get((struct k_json_obj *)json, key, strlen(key));
    if (NULL == pair)
        return NULL;
    else
        return pair->val;
}

struct k_json *k_json_deep_get(struct k_json *json, ...) {

    if (NULL == json || K__JSON_OBJ != json->type)
        return NULL;

    if (K__JSON_OBJ != json->type && K__JSON_ARR != json->type)
        return NULL;

    struct k_json *result = json;
    void *fn_get = NULL;

    va_list args;
    va_start(args, json);
    while (NULL != (fn_get = va_arg(args, void *))) {

        if (NULL == result)
            break;

        if (fn_get == (void *)k_json_arr_get) {
            size_t idx = va_arg(args, size_t);
            result = k_json_arr_get(result, idx);
        }
        else if (fn_get == (void *)k_json_obj_get) {
            const char *key = va_arg(args, const char *);
            result = k_json_obj_get(result, key);
        }
        else {
            result = NULL;
            break;
        }
    }

    va_end(args);
    return result;
}

void k_json_arr_del(struct k_json *json, size_t idx) {

    if (NULL == json || K__JSON_ARR != json->type)
        return;

    k__json_arr_del((struct k_json_arr *)json, idx);
}

void k_json_obj_del(struct k_json *json, const char *key) {

    if (NULL == json || K__JSON_OBJ != json->type)
        return;

    if (NULL == key)
        return;

    k__json_obj_del((struct k_json_obj *)json, key);
}

size_t k_json_arr_get_size(struct k_json *json) {
    assert(NULL != json);
    assert(K__JSON_ARR == json->type);

    return k__json_arr_get_size((struct k_json_arr *)json);
}

int k_json_num_get_i(struct k_json *json) {
    assert(K__JSON_NUM == json->type);
    return ((struct k_json_num *)json)->num_i;
}

double k_json_num_get_f(struct k_json *json) {
    assert(K__JSON_NUM == json->type);
    return ((struct k_json_num *)json)->num_f;
}

const char *k_json_num_get_s(struct k_json *json) {
    assert(K__JSON_NUM == json->type);
    return k__json_num_get_s((struct k_json_num *)json);
}

int k_json_bool_get(struct k_json *json) {
    assert(K__JSON_BOOL == json->type);
    return ((struct k_json_bool *)json)->is_true;
}

const char *k_json_str_get(struct k_json *json) {
    assert(K__JSON_STR == json->type);
    return ((struct k_json_str *)json)->str;
}
