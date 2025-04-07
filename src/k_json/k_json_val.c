
#include "./_internal.h"

struct k__json_null *k__create_json_null(void) {

    static struct k__json_null json_null = {
        .json.type = K__JSON_NULL
    };

    return &json_null;
}

void k__destroy_json_null(struct k__json_null *json_null) {
    /* do nothing */
}

struct k__json_bool *k__create_json_bool(int is_true) {

    static struct k__json_bool json_true = {
        .json.type = K__JSON_BOOL,
        .is_true = 1
    };

    static struct k__json_bool json_false = {
        .json.type = K__JSON_BOOL,
        .is_true = 0
    };

    return is_true ? &json_true : &json_false;
}

void k__destroy_json_bool(struct k__json_bool *json_bool) {
    /* do nothing */
}
