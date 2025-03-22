#include <limits.h>
#include <stdlib.h>
#include <math.h>
#include "k_json.h"
#include "_internal.h"

struct k__json_num *k__create_json_num_i(int num) {

    struct k__json_num *json_num = k__json_mem_alloc(sizeof(struct k__json_num));
    if (json_num == NULL)
        return NULL;

    json_num->json.type = K_JSON_NUM;
    json_num->num_i = num;
    json_num->num_s = NULL;
    json_num->num_f = (double)num;

    return json_num;
}

struct k__json_num *k__create_json_num_f(double num) {

    if (isnan(num) || isinf(num))
        return NULL;

   struct k__json_num *json_num = k__json_mem_alloc(sizeof(struct k__json_num));
    if (json_num == NULL)
        return NULL;

    json_num->json.type = K_JSON_NUM;
    json_num->num_s = NULL;
    json_num->num_f = num;
    json_num->num_i = num <= (double)INT_MIN ? INT_MIN :
                      num >= (double)INT_MAX ? INT_MAX : (int)num;

    return json_num;
}

struct k__json_num *k__create_json_num_f_s(const char *num, size_t len) {

    char *end;
    double num_f = strtod(num, &end);
    if (end - num != len)
        return NULL; /* 要求数字末尾位于 num[len] */

    int num_i = num_f <= (double)INT_MIN ? INT_MIN :
                num_f >= (double)INT_MAX ? INT_MAX : (int)num_f;

    struct k__json_num *json_num = k__json_mem_alloc(sizeof(struct k__json_num));
    if (NULL == json_num)
        return NULL;

    char *num_copy = json_num->buf;
    if (len < sizeof(json_num->buf)) {
        memcpy(num_copy, num, len);
        num_copy[len] = '\0';
    }
    else {
        char *alloc_buf = k__json_strdup(num, len);
        if (NULL == alloc_buf) {
            k__destroy_json_num(json_num);
            return NULL;
        }

        num_copy = alloc_buf;
    }

    json_num->json.type = K_JSON_NUM;
    json_num->num_s = num_copy;
    json_num->num_f = num_f;
    json_num->num_i = num_i;

    return json_num;
}

struct k__json_num *k__create_json_num_i_s(const char *num, size_t len) {

    char *end;
    long num_l = strtol(num, &end, 10);
    if (end - num != len)
        return NULL; /* 要求数字末尾位于 num[len] */

    int num_i = num_l <= INT_MIN ? INT_MIN :
                num_l >= INT_MAX ? INT_MAX : (int)num_l;

    struct k__json_num *json_num = k__json_mem_alloc(sizeof(struct k__json_num));
    if (NULL == json_num)
        return NULL;

    char *num_copy = json_num->buf;
    if (len < sizeof(json_num->buf)) {
        memcpy(num_copy, num, len);
        num_copy[len] = '\0';
    }
    else {
        char *alloc_buf = k__json_strdup(num, len);
        if (NULL == alloc_buf) {
            k__destroy_json_num(json_num);
            return NULL;
        }

        num_copy = alloc_buf;
    }

    json_num->json.type = K_JSON_NUM;
    json_num->num_s = num_copy;
    json_num->num_i = num_i;
    json_num->num_f = (double)num_i;

    return json_num;
}

void k__destroy_json_num(struct k__json_num *json_num) {

    if (json_num->num_s != json_num->buf)
        k__json_mem_free(json_num->num_s);

    k__json_mem_free(json_num);
}

const char *k__json_num_get_s(struct k__json_num *json_num) {

    if (json_num->num_s != NULL)
        return json_num->num_s;

    int str_len;
    if ((double)json_num->num_i == json_num->num_f) {
        str_len = snprintf(json_num->buf, sizeof(json_num->buf), "%d", json_num->num_i);
    } else {
        str_len = snprintf(json_num->buf, sizeof(json_num->buf), "%g", json_num->num_f);
    }

    if (str_len < 0)
        return NULL;

    if (str_len < sizeof(json_num->buf)) {
        json_num->num_s = json_num->buf;
    }
    else {
        char *alloc_buf = k__json_mem_alloc(str_len + 1);
        if (NULL == alloc_buf)
            return NULL;

        if ((double)json_num->num_i == json_num->num_f)
            snprintf(alloc_buf, str_len, "%d", json_num->num_i);
        else
            snprintf(alloc_buf, str_len, "%g", json_num->num_f);

        json_num->num_s = alloc_buf;
    }

    return json_num->num_s;
}
