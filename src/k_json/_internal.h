#ifndef K__JSON_INTERNAL_H
#define K__JSON_INTERNAL_H

#include <string.h>
#include <stdint.h>

#include "k_hash_list.h"
#include "k_list.h"
#include "k_printf.h"

#include "k_json.h"

void *k__json_mem_alloc(size_t size);
void  k__json_mem_free(void *ptr);
char *k__json_strdup(const char *str, size_t len);

enum k_json_type {
    K__JSON_NULL ,
    K__JSON_BOOL ,
    K__JSON_STR  ,
    K__JSON_NUM  ,
    K__JSON_ARR  ,
    K__JSON_OBJ  ,
};

struct k_json {
    enum k_json_type type;
};

/* region [null & bool] */

struct k__json_null {
    struct k_json json;
};

struct k__json_null *k__create_json_null(void);
void k__destroy_json_null(struct k__json_null *json_null);

struct k__json_bool {
    struct k_json json;
    int is_true;
};

struct k__json_bool *k__create_json_bool(int is_true);
void k__destroy_json_bool(struct k__json_bool *json_bool);

/* endregion */

/* region [num] */

struct k__json_num {
    struct k_json json;
    int     num_i;
    double  num_f;
    char   *num_s;
    char    buf[24];
};

struct k__json_num *k__create_json_num_i(int num);
struct k__json_num *k__create_json_num_f(double num);
struct k__json_num *k__create_json_num_i_s(const char *num, size_t len);
struct k__json_num *k__create_json_num_f_s(const char *num, size_t len);
void k__destroy_json_num (struct k__json_num  *json_num);
const char *k__json_num_get_s(struct k__json_num *json_num);

/* endregion */

/* region [str] */

struct k__json_str {
    struct k_json json;
    char *str;
};

struct k__json_str *k__create_json_str(const char *str, size_t len);
void k__destroy_json_str (struct k__json_str  *json_str);

/* endregion */

/* region [arr] */

struct k__json_arr {
    struct k_json json;
    size_t capacity;
    size_t size;
    struct k_json **arr;
};

struct k__json_arr *k__create_json_arr(void);
void k__destroy_json_arr(struct k__json_arr *json_arr);
int k__json_arr_add(struct k__json_arr *json_arr, size_t idx, struct k_json *val);
int k__json_arr_push(struct k__json_arr *json_arr, struct k_json *val);
struct k_json *k__json_arr_get(struct k__json_arr *json_arr, size_t idx);
void k__json_arr_del(struct k__json_arr *json_arr, size_t idx);
size_t k__json_arr_get_size(struct k__json_arr *json_arr);

/* endregion */

/* region [obj] */

struct k__json_obj {
    struct k_json json;
    struct k_hash_list *lists;
    size_t lists_num;
};

struct k__json_obj_pair {
    struct k_hash_list_node link;
    char *key;
    size_t key_hash;
    struct k_json *val;
};

struct k__json_obj  *k__create_json_obj (void);
void k__destroy_json_obj (struct k__json_obj  *json_obj);
int k__json_obj_add (struct k__json_obj *json_obj, const char *key, size_t key_len, struct k_json *val);
struct k__json_obj_pair *k__json_obj_get(struct k__json_obj *json_obj, const char *key, size_t key_len);
void k__json_obj_del(struct k__json_obj *json_obj, const char *key);

/* endregion */

struct k_json *k__json_parse(const char *fmt, va_list args);
void k__destroy_json(struct k_json *json);

#endif
