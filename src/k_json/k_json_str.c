
#include "./_internal.h"

struct k_json_str *k__json_create_str(const char *str, size_t len) {
    char *str_copy = NULL;
    struct k_json_str *json_str = NULL;

    str_copy = k__json_strdup(str, len);
    json_str = k__json_mem_alloc(sizeof(struct k_json_str));
    if (str_copy == NULL || json_str == NULL)
        goto err;

    json_str->json.type = K__JSON_STR;
    json_str->str = str_copy;
    return json_str;

err:
    k__json_mem_free(str_copy);
    k__json_mem_free(json_str);
    return NULL;
}

void k__json_destroy_str(struct k_json_str *json_str) {
    k__json_mem_free(json_str->str);
    k__json_mem_free(json_str);
}
