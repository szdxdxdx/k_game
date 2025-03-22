#ifndef K_JSON_H
#define K_JSON_H

#include <stdio.h>

#include "k_printf.h"

enum k_json_type {
    K_JSON_NULL ,
    K_JSON_BOOL ,
    K_JSON_STR  ,
    K_JSON_NUM  ,
    K_JSON_ARR  ,
    K_JSON_OBJ  ,
};

struct k_json {
    enum k_json_type type;
};

/**
 * \brief 格式化解析字符串，构建 json 对象
 *
 *
 * 格式字符串 `fmt` 用于指定 json 字符串的格式，
 * 支持的格式说明符、对应实参的类型，以及转换结果如下：
 * ```
 *  - %d | int             | 整数
 *  - %f | double          | 浮点数
 *  - %b | int             | 布尔值
 *  - %s | const char *    | 字符串
 *  - %j | struct k_json * | json 子对象或 null
 * ```
 *
 * 其中，`%s` 只能对应 json 格式的的字符串值，
 * 不能作为 object 的 key，也不能用作对格式字符串 `fmt` 的再补充。
 *
 * ```C
 *
 * // 构建的 json 对象是一个 string，而不是包含了 3 个 number 的 array
 * k_json_parse("%s", "[ 1, 2, 3 ]");
 *
 * // 构建 json 失败，%s 不支持作为 object 的 key
 * k_json_parse("{ %s : 1 }", "num");
 *
 * // 构建 json 失败，%s 不能用作格式字符串 `fmt` 的再补充
 * k_json_parse("[ 1, 2 %s", ", 3]");
 * ```
 *
 * 函数不是把参数按格式说明符组合成一个完整的字符串后再解析，
 * 而是边解析边处理参数。
 *
 * 若成功，函数返回 json 对象指针，否则返回 NULL。
 */
struct k_json *k_json_parse(const char *fmt, ...);

void k_json_free(struct k_json *json);

int k_json_num_get_i(struct k_json *json);

double k_json_num_get_f(struct k_json *json);

const char *k_json_num_get_s(struct k_json *json);

int k_json_bool_get(struct k_json *json);

const char *k_json_str_get(struct k_json *json);

int k_json_arr_add(struct k_json *json, size_t idx, struct k_json *val);
int k_json_obj_add(struct k_json *json, const char *key, struct k_json *val);
int k_json_arr_push(struct k_json *json, struct k_json *val);

void k_json_arr_del(struct k_json *json, size_t idx);
void k_json_obj_del(struct k_json *json, const char *key);

struct k_json *k_json_arr_get(struct k_json *json, size_t idx);
struct k_json *k_json_obj_get(struct k_json *json, const char *key);

#define k_json_get_by_idx(idx) k_json_arr_get, (size_t)(idx)
#define k_json_get_by_key(key) k_json_obj_get, (char *)(key)
struct k_json *k_json_get(struct k_json *json, ...);

int k_json_get_i(struct k_json *json, int *get_i);
int k_json_get_f(struct k_json *json, double *get_f);
int k_json_get_s(struct k_json *json, const char **get_s);

void k_printf_callback_k_json(struct k_printf_buf *buf, const struct k_printf_spec *spec, va_list *args);

#endif
