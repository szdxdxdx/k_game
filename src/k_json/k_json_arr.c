#include "k_array.h"

#include "./_internal.h"

struct k__json_arr *k__create_json_arr(void) {

    struct k__json_arr *json_arr = k__json_mem_alloc(sizeof(struct k__json_arr));
    if (json_arr == NULL)
        return NULL;

    json_arr->json.type = K__JSON_ARR;
    json_arr->capacity  = 0;
    json_arr->size      = 0;
    json_arr->arr       = NULL;

    return json_arr;
}

void k__destroy_json_arr(struct k__json_arr *json_arr) {

    size_t i = 0;
    for (; i < json_arr->size; i++)
        k__destroy_json(json_arr->arr[i]);

    k__json_mem_free(json_arr->arr);
    k__json_mem_free(json_arr);
}

int k__json_arr_add(struct k__json_arr *json_arr, size_t idx, struct k_json *val) {

    if (json_arr->size < idx)
        return -1;

    struct k_array arr; /* <- tmp */
    arr.fn_malloc = k__json_mem_alloc;
    arr.fn_free   = k__json_mem_free;
    arr.elem_size = sizeof(struct k_json *);
    arr.capacity  = json_arr->capacity;
    arr.size      = json_arr->size;
    arr.storage   = json_arr->arr;
    if (0 != k_array_insert(&arr, idx, &val))
        return -1;

    json_arr->capacity = arr.capacity;
    json_arr->size     = arr.size;
    json_arr->arr      = arr.storage;

    return 0;
}

int k__json_arr_push(struct k__json_arr *json_arr, struct k_json *val) {
    return k__json_arr_add(json_arr, json_arr->size, val);
}

struct k_json *k__json_arr_get(struct k__json_arr *json_arr, size_t idx) {
    return json_arr->size <= idx ? NULL : json_arr->arr[idx];
}

void k__json_arr_del(struct k__json_arr *json_arr, size_t idx) {
    if (json_arr->size <= idx)
        return;

    struct k_json *val = json_arr->arr[idx];

    struct k_array arr;
    arr.fn_malloc = k__json_mem_alloc;
    arr.fn_free   = k__json_mem_free;
    arr.elem_size = sizeof(struct k_json *);
    arr.capacity  = json_arr->capacity;
    arr.size      = json_arr->size;
    arr.storage   = json_arr->arr;

    k_array_remove(&arr, idx);

    json_arr->capacity = arr.capacity;
    json_arr->size     = arr.size;

    k__destroy_json(val);
}

size_t k__json_arr_get_size(struct k__json_arr *json_arr) {
    return json_arr->size;
}
