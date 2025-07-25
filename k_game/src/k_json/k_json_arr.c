
#define k_tmpl_array_set_fn_malloc k__json_mem_alloc
#define k_tmpl_array_set_fn_free   k__json_mem_free
#define K_TMPL_ARRAY_DEFINE_FUNCTION
#include "./_internal.h"

struct k_json_arr *k__json_arr_create(void) {

    struct k_json_arr *json_arr = k__json_mem_alloc(sizeof(struct k_json_arr));
    if (json_arr == NULL)
        return NULL;

    json_arr->json.type = K__JSON_ARR;
    k_json_array_construct(&json_arr->arr_);

    return json_arr;
}

void k__json_arr_destroy(struct k_json_arr *json_arr) {

    size_t i = 0;
    for (; i < json_arr->arr_.size; i++) {
        k__json_destroy(json_arr->arr_.storage[i]);
    }

    k_json_array_destruct(&json_arr->arr_);
    k__json_mem_free(json_arr);
}

int k__json_arr_add(struct k_json_arr *json_arr, size_t idx, struct k_json *val) {
    return k_json_array_insert(&json_arr->arr_, idx, val);
}

int k__json_arr_push(struct k_json_arr *json_arr, struct k_json *val) {
    return k_json_array_push_back(&json_arr->arr_, val);
}

struct k_json *k__json_arr_get(struct k_json_arr *json_arr, size_t idx) {
    return json_arr->arr_.size <= idx ? NULL : json_arr->arr_.storage[idx];
}

void k__json_arr_del(struct k_json_arr *json_arr, size_t idx) {
    if (json_arr->arr_.size <= idx)
        return;

    struct k_json *val = json_arr->arr_.storage[idx];

    k_json_array_remove(&json_arr->arr_, idx);

    k__json_destroy(val);
}

size_t k__json_arr_get_size(struct k_json_arr *json_arr) {
    return json_arr->arr_.size;
}
