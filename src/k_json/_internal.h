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

struct k_json_null {
    struct k_json json;
};

struct k_json_null *k__json_null_create(void);
void k__json_null_destroy(struct k_json_null *json_null);

struct k_json_bool {
    struct k_json json;
    int is_true;
};

struct k_json_bool *k__json_bool_create(int is_true);
void k__json_bool_destroy(struct k_json_bool *json_bool);

/* endregion */

/* region [num] */

struct k_json_num {
    struct k_json json;
    int    num_i;
    float  num_f;
    char  *num_s;
    char   buf[24];
};

struct k_json_num *k__json_num_create_i(int num);
struct k_json_num *k__json_num_create_f(float num);
struct k_json_num *k__json_num_create_i_s(const char *num, size_t len);
struct k_json_num *k__json_num_create_f_s(const char *num, size_t len);
void k__json_num_destroy(struct k_json_num  *json_num);
const char *k__json_num_get_s(struct k_json_num *json_num);

/* endregion */

/* region [str] */

struct k_json_str {
    struct k_json json;
    char *str;
};

struct k_json_str *k__json_create_str(const char *str, size_t len);
void k__json_destroy_str (struct k_json_str  *json_str);

/* endregion */

/* region [arr] */

#define k_tmpl_array_struct_name k_json_array
#define k_tmpl_array_elem_type   struct k_json *
#define k_tmpl_array_define_struct
#include "k_array_tmpl.h"

struct k_json_arr {
    struct k_json json;
    struct k_json_array arr_;
};

struct k_json_arr *k__json_arr_create(void);
void k__json_arr_destroy(struct k_json_arr *json_arr);
int k__json_arr_add(struct k_json_arr *json_arr, size_t idx, struct k_json *val);
int k__json_arr_push(struct k_json_arr *json_arr, struct k_json *val);
struct k_json *k__json_arr_get(struct k_json_arr *json_arr, size_t idx);
void k__json_arr_del(struct k_json_arr *json_arr, size_t idx);
size_t k__json_arr_get_size(struct k_json_arr *json_arr);

/* endregion */

/* region [obj] */

struct k_json_obj {
    struct k_json json;
    struct k_hash_list *lists;
    size_t lists_num;
};

struct k_json_obj_pair {
    struct k_hash_list_node link;
    char *key;
    size_t key_hash;
    struct k_json *val;
};

struct k_json_obj  *k__json_obj_create (void);
void k__json_obj_destroy (struct k_json_obj *json_obj);
int k__json_obj_add (struct k_json_obj *json_obj, const char *key, size_t key_len, struct k_json *val);
struct k_json_obj_pair *k__json_obj_get(struct k_json_obj *json_obj, const char *key, size_t key_len);
void k__json_obj_del(struct k_json_obj *json_obj, const char *key);

/* endregion */

struct k_json *k__json_parse(const char *fmt, va_list args);
void k__json_destroy(struct k_json *json);

#endif
