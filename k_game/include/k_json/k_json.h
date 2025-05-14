#ifndef K_JSON_H
#define K_JSON_H

#include <stddef.h>



/*
 * [!] 【功能不完善】的 json 解析器
 *
 * 不支持解析带中文的文本
 * 不支持字符串转义
 *
 * ---
 *
 * 该模块是我在项目的早期阶段写的。当时项目经验不足，编码经验也少，
 * api 设计不简洁，不好用，模块的内部结构体、函数设计得不好，
 * 中途改过几版，但都不太满意。我代码写得啰嗦，改起来很也困难。
 * 目前它是一个摇摇欲坠的模块。我之后会重构它，api 一定会有所变动。
 */



/** \brief json 节点 */
struct k_json;

/**
 * \brief 格式化解析字符串，构建 json 对象
 *
 * `fmt` 为待解析的 json 格式的字符串，支持使用格式说明符占位。
 * 支持的格式说明符、对应实参的类型，以及转换结果如下：
 *  - %d `int`             整数
 *  - %f `double`          浮点数
 *  - %b `int`             布尔值
 *  - %s `const char *`    字符串
 *  - %j `struct k_json *` json 子对象或 null
 *
 * 其中，`%s` 只能对应 json 格式的的字符串值，
 * 不能作为 object 的 key，也不能用作对格式字符串 `fmt` 的再补充。
 * 函数不是把参数按格式说明符组合成一个完整的字符串后再解析，
 * 而是边解析边处理参数。解析完成后，原字符串
 *
 * 示例：
 * ```C
 * // 创建一个空值
 * k_json_parse("null");
 *
 * // 创建一个布尔值
 * k_json_parse("true");
 * k_json_parse("false");
 *
 * // 创建一个数字
 * k_json_parse("1");
 * k_json_parse("2.0");
 * k_json_parse("-3.e4");
 *
 * // 创建一个字符串，注意字符串要有双引号包裹
 * k_json_parse("\"str\"");
 *
 * // 创建一个列表
 * k_json_parse("[]");
 *
 * // 创建一个带有元素的列表
 * k_json_parse("[ 1, 2, 3 ]");
 *
 * // 创建一个对象
 * k_json_parse("{}");
 *
 * // 创建一个带有属性的对象，注意 key 字符串要有双引号包裹
 * k_json_parse("{ \"key\": "value" }");
 *
 * // 解析带有格式说明符的 json 文本
 * k_json_parse("[ %d, %f, %b, %s ]", 123, 3.14, 0, "hello");
 *
 * // 构建 json 失败，%s 不支持作为 object 的 key
 * k_json_parse("{ %s : 1 }", "num");
 *
 * // 构建 json 失败，%s 不能用作格式字符串 `fmt` 的再补充
 * k_json_parse("[ 1, 2 %s", ", 3]");
 * ```
 *
 * 若解析成功，函数返回 json 对象指针，否则返回 NULL。
 */
struct k_json *k_json_parse(const char *fmt, ...);

/**
 * \brief 释放 json 结构
 *
 * 释放 json 结构，请确保传入的是根节点。
 *
 * 若 `json` 为 `NULL`，则函数立即返回。
 */
void k_json_free(struct k_json *json);

/* region [arr] */

/** \brief 往 json 列表节点中的指定索引处插入一个元素。添加成功返回 0，否则返回非 0 */
int k_json_arr_add(struct k_json *json, size_t idx, struct k_json *val);

/** \brief 往 json 列表节点尾部追加一个元素。追加成功返回 0，否则返回非 0 */
int k_json_arr_push(struct k_json *json, struct k_json *val);

/** \brief 获取 json 列表节点中指定索引处的元素 */
struct k_json *k_json_arr_get(struct k_json *json, size_t idx);

/** \brief 删除 json 列表节点中指定索引处的元素 */
void k_json_arr_del(struct k_json *json, size_t idx);

/** \brief 获取 json 列表节点的元素个数 */
size_t k_json_arr_get_size(struct k_json *json);

/* endregion */

/* region [obj] */

/** \brief 往 json 对象节点中添加一个元素。添加成功返回 0，否则返回非 0 */
int k_json_obj_add(struct k_json *json, const char *key, struct k_json *val);

/** \brief 删除 json 对象节点中指定键的元素 */
void k_json_obj_del(struct k_json *json, const char *key);

/** \brief 获取 json 对象节点中指定键的元素 */
struct k_json *k_json_obj_get(struct k_json *json, const char *key);

/* endregion */

/* region [get] */

/** \brief 获取 json 数字节点的值（整型形式） */
int k_json_num_get_i(struct k_json *json);

/** \brief 获取 json 数字节点的值（浮点形式） */
double k_json_num_get_f(struct k_json *json);

/** \brief 获取 json 数字节点的值（字符串形式） */
const char *k_json_num_get_s(struct k_json *json);

/** \brief 获取 json 布尔节点的值，1 表示 true，0 表示 false */
int k_json_bool_get(struct k_json *json);

/** \brief 获取 json 字符串节点的值，你不要释放该字符串 */
const char *k_json_str_get(struct k_json *json);

/**
 * \brief 获取列表节点或对象节点的深层子结点
 *
 * 请保证传入的是列表节点或对象节点。
 *
 * ```C
 * struct k_json *node; // <- 要获取该节点的深层子节点，类似于 node["k_1"]["k_2"][3]["k_4"][5]
 *
 * struct k_json *deep_child = k_json_deep_get(
 *     node,
 *     k_json_get_by_key("k_1"),
 *     k_json_get_by_key("k_2"),  // 使用 `k_json_get_by_key()` 宏，获取对象节点的元素
 *     k_json_get_by_idx(3),      // 使用 `k_json_get_by_idx()` 宏，获取列表节点的元素
 *     k_json_get_by_key("k_4"),
 *     k_json_get_by_idx(5),
 *     NULL                       // 以 `NULL` 作为哨兵值，表示结束
 * );
 * ```
 */
struct k_json *k_json_deep_get(struct k_json *json, ...);
#define k_json_get_by_idx(idx) k_json_arr_get, (size_t)(idx)
#define k_json_get_by_key(key) k_json_obj_get, (char *)(key)

/* endregion */

#endif
