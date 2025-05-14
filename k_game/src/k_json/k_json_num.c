#include <limits.h>
#include <stdlib.h>
#include <math.h>

#include "./_internal.h"

struct k_json_num *k__json_num_create_i(int num) {

    struct k_json_num *json_num = k__json_mem_alloc(sizeof(struct k_json_num));
    if (json_num == NULL)
        return NULL;

    json_num->json.type = K__JSON_NUM;
    json_num->num_i = num;
    json_num->num_s = NULL;
    json_num->num_f = (float)num;

    return json_num;
}

struct k_json_num *k__json_num_create_f(float num) {

    if (isnan(num) || isinf(num))
        return NULL;

   struct k_json_num *json_num = k__json_mem_alloc(sizeof(struct k_json_num));
    if (json_num == NULL)
        return NULL;

    json_num->json.type = K__JSON_NUM;
    json_num->num_s = NULL;
    json_num->num_f = num;
    json_num->num_i = num <= (float)INT_MIN ? INT_MIN :
                      num >= (float)INT_MAX ? INT_MAX : (int)num;

    return json_num;
}

struct k_json_num *k__json_num_create_f_s(const char *num, size_t len) {

    char *end;
    float num_f = (float)strtod(num, &end);
    if (end - num != len)
        return NULL; /* 要求数字末尾位于 num[len] */

    int num_i = num_f <= (float)INT_MIN ? INT_MIN :
                num_f >= (float)INT_MAX ? INT_MAX : (int)num_f;

    struct k_json_num *json_num = k__json_mem_alloc(sizeof(struct k_json_num));
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
            k__json_num_destroy(json_num);
            return NULL;
        }

        num_copy = alloc_buf;
    }

    json_num->json.type = K__JSON_NUM;
    json_num->num_s = num_copy;
    json_num->num_f = num_f;
    json_num->num_i = num_i;

    return json_num;
}

struct k_json_num *k__json_num_create_i_s(const char *num, size_t len) {

    char *end;
    long num_l = strtol(num, &end, 10);
    if (end - num != len)
        return NULL; /* 要求数字末尾位于 num[len] */

    int num_i = num_l <= INT_MIN ? INT_MIN :
                num_l >= INT_MAX ? INT_MAX : (int)num_l;

    struct k_json_num *json_num = k__json_mem_alloc(sizeof(struct k_json_num));
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
            k__json_num_destroy(json_num);
            return NULL;
        }

        num_copy = alloc_buf;
    }

    json_num->json.type = K__JSON_NUM;
    json_num->num_s = num_copy;
    json_num->num_i = num_i;
    json_num->num_f = (float)num_i;

    return json_num;
}

void k__json_num_destroy(struct k_json_num *json_num) {

    if (json_num->num_s != json_num->buf)
        k__json_mem_free(json_num->num_s);

    k__json_mem_free(json_num);
}

const char *k__json_num_get_s(struct k_json_num *json_num) {

    if (json_num->num_s != NULL)
        return json_num->num_s;

    int str_len;
    if ((float)json_num->num_i == json_num->num_f) {
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

        if ((float)json_num->num_i == json_num->num_f)
            snprintf(alloc_buf, str_len, "%d", json_num->num_i);
        else
            snprintf(alloc_buf, str_len, "%g", json_num->num_f);

        json_num->num_s = alloc_buf;
    }

    return json_num->num_s;
}
