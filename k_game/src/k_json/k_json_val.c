
#include "./_internal.h"

struct k_json_null *k__json_null_create(void) {

    static struct k_json_null json_null = {
        .json.type = K__JSON_NULL
    };

    return &json_null;
}

void k__json_null_destroy(struct k_json_null *json_null) {
    /* do nothing */
}

struct k_json_bool *k__json_bool_create(int is_true) {

    static struct k_json_bool json_true = {
        .json.type = K__JSON_BOOL,
        .is_true = 1
    };

    static struct k_json_bool json_false = {
        .json.type = K__JSON_BOOL,
        .is_true = 0
    };

    return is_true ? &json_true : &json_false;
}

void k__json_bool_destroy(struct k_json_bool *json_bool) {
    /* do nothing */
}
