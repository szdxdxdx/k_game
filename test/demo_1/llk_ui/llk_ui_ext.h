#ifndef LLK_UI_EXT_H
#define LLK_UI_EXT_H

#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "k_mem_pool.h"
#include "k_str_map.h"
#include "k_list.h"

#include "./llk_ui.h"

/* region [val] */

/* 表示一个携带单位的 float 类型的值 */
struct llk_ui_float {

    /* 指定的值 */
    float specified_val;

    /* 指定的值的单位，取自枚举 `llk_ui_unit` */
    int unit;

    /* 计算后的值
     *
     * 长度相关的值，计算后统一单位为：像素
     */
    float computed_val;
};

/* 表示一个携带单位的 uint32_t 类型的值 */
struct llk_ui_u32 {

    /* 指定的值 */
    uint32_t specified_val;

    /* 指定的值的单位，取自枚举 `llk_ui_unit` */
    int unit;

    /* 计算后的值 */
    uint32_t computed_val;
};

/* 值的单位 */
enum llk_ui_unit {

    /* 表示 `specified_val` 没有被赋值
     *
     * 此时 `computed_val` 的值无意义
     */
    LLK_UI_UNIT_NO_VAL,

    /* 像素
     *
     * 例如：`50px`、`100px`
     *
     * 若指定的值不带有单位，则默认单位为像素。
     */
    LLK_UI_UNIT_PX,

    /* 相对于父元素的宽高比
     *
     * 例如：`50.0%`、`100%`
     *
     * 字符串值的取值的 `100%，对应解析后的值为 1.0f。
     */
    LLK_UI_UNIT_PERCENT,

    /* 相对于视窗的宽高比
     *
     * 例如： `50vw`、`100vh`
     *
     * 字符串值的取值的 `100%，对应解析后的值为 1.0f。
     */
    LLK_UI_UNIT_VW,
    LLK_UI_UNIT_VH,

    /* 十六进制颜色值
     *
     * 例如：
     * - 不带透明度 RGB `#ff6600`
     * - 带透明度 RGBA `#ee000066`
     *
     * 对应解析后的值为 uint32_t 的 0xRRGGBBAA 十六进制颜色值
     */
    LLK_UI_UNIT_RGBA,

    /* 用于自定义单位 */
    LLK_UI_UNIT_CUSTOM,
};

/* 检查 `llk_ui_float` 或 `llk_ui_u32` 类型的值是否已被指定 */
#define llk__ui_is_val_specified(val) \
    ((val).unit != LLK_UI_UNIT_NO_VAL)

/* endregion */

/* region [UI_context] */

/* UI 上下文 */
struct llk_ui_context {

    /* 视口宽高，即 UI 画布的宽高 */
    float vw;
    float vh;

    /* 鼠标在视口的坐标 */
    float mouse_x;
    float mouse_y;

    /* 记录鼠标左键是否按下或按住 */
    unsigned int mouse_button_pressed;
    unsigned int mouse_button_down;

    /* 根元素节点，elem type 固定是 `root` */
    struct llk_ui_elem *root;

    /* TODO 记录由该上下文创建出的，但还没有添加到树中的（没有父元素）的节点 */

    /* 内存池 */
    struct k_mem_pool mem_pool;

    /* 记录已注册的元素类型 */
    struct k_str_map elem_type_map;

    /* 用于通过 id 查找元素 */
    struct k_str_intrusive_map elem_id_map;

    /* 记录已注册的函数 */
    struct k_str_map callback_fn_map;

    /* 布局脏标记。若为非 0，则重新计算所有元素的尺寸和位置 */
    int layout_dirty;
};

/* 标记 UI 结构或布局发生了变更，需要重新布局 */
void llk__ui_mark_layout_dirty(struct llk_ui_context *ui);

/* 刷新布局：计算所有元素的尺寸，定位元素的位置，并判断鼠标指针落在哪个元素上 */
void llk__ui_refresh(struct llk_ui_context *ui);

/* 若布局脏标记为非 0，则刷新布局 */
void llk__ui_refresh_if_layout_dirty(struct llk_ui_context *ui);

/* endregion */

/* region [UI_elem_type] */

struct llk_ui_elem_type;

/* 注册自定义的 UI 元素类型
 *
 * 只有注册类型成功后，才能通过 `llk_ui_create_elem()` 创建该类型的 UI 元素实例。
 * 函数会复制一份 `type` 的值。
 *
 * 若注册成功，函数返回 0，否则返回非 0。
 */
int llk__ui_register_elem_type(struct llk_ui_context *ui, const struct llk_ui_elem_type *type);

/* UI 元素类型信息 */
struct llk_ui_elem_type {

    /* 类型的名字 */
    const char *type_name;

    /* 指定类型的自定义数据段的大小
     *
     * 创建实例时，根据该字段分配 `llk_ui_elem` 的 `data` 的内存。
     * 若为 0，则表示该类型的元素没有自定义数据。
     */
    size_t data_size;

    /* 初始化函数，在创建实例时调用
     *
     * 若初始化成功，函数应返回 0，否则应返回非 0。
     */
    int (*fn_init)(struct llk_ui_elem *elem);

    /* 析构函数，在销毁实例时调用 */
    void (*fn_fini)(struct llk_ui_elem *elem);

    /* 设置属性的函数，在设置属性时调用
     *
     * 你需要先自己解析 key，来判断要设置的是否是该类型的属性，
     * 若是，你需要自己解析 val 来设置值；若设置成功，函数应返回 0，否则应返回非 0。
     *
     * 若不是该类型的属性，你应调用 `llk__ui_elem_set_attr_default()` 交由基类处理。
     */
    int (*fn_set_attr)(struct llk_ui_elem *elem, const char *key, const char *val);

    /* 绘制函数，在需要绘制该元素时被调用
     *
     * 交由你来绘制元素，请绘制在合理的地方。
     * 若设为 `NULL` 则忽略绘制，表明该元素仅用于定位布局。
     *
     * 绘制的顺序是，先绘制父元素，再绘制子元素。
     * 你不需要主动通知子元素执行绘制。
     */
    void (*fn_draw)(struct llk_ui_elem *elem);

    /* 事件分发函数，在分发事件时调用
     *
     * 交由你来处理事件分发事件。
     * 交由你来设计该类型的元素支持什么样的回调，并由你来发起调用。
     *
     * 你不需要手动通知子元素的绘制函数。
     *
     * 事件分发的顺序是，先出发子元素的事件回调，再轮到父元素。
     * 你不需要主动通知父元素执行事件回调。
     */
    void (*fn_dispatch_event)(struct llk_ui_elem *elem);
};

/* endregion */

/* region [UI_elem] */

/* UI 元素节点 */
struct llk_ui_elem {

    /* 所属的上下文 */
    struct llk_ui_context *ui;

    /* 父节点 */
    struct llk_ui_elem *parent;

    /* 子节点列表 */
    struct k_list child_list;

    /* 指向相邻兄弟节点的指针域 */
    struct k_list_node sibling_link;

    /* 该元素的类型信息，以及该类型的自定义数据 */
    const struct llk_ui_elem_type *type;
    void *data;

    /* 侵入的哈希表节点，属于 `llk_ui_context->elem_id_map` */
    struct k_str_intrusive_map_node id_map_node;

    /* 元素的宽高 */
    struct llk_ui_float w;
    struct llk_ui_float h;

    /* 相对于父元素的定位（绝对定位的布局方式） */
    struct llk_ui_float left;
    struct llk_ui_float right;
    struct llk_ui_float top;
    struct llk_ui_float bottom;

    /* 元素左上角在 UI 界面中的坐标（计算布局后得出） */
    float x;
    float y;

    /* 标记鼠标指针是否悬浮在该元素上 */
    unsigned int is_hovered;

    /* 标记该元素（及其子元素）是否显示 */
    unsigned int is_hidden;
};

/* 设置 UI 元素的属性的默认实现 */
int llk__ui_elem_set_attr_default(struct llk_ui_elem *elem, const char *key, const char *val);

/* 计算 UI 元素及其子元素的宽高尺寸 */
void llk__ui_elem_measure(struct llk_ui_elem *elem);

/* 定位 UI 元素及其子元素的位置坐标 */
void llk__ui_elem_layout(struct llk_ui_elem *elem);

/* 判断输入事件（鼠标点击、触摸）是否命中 UI 元素及其子元素 */
void llk__ui_elem_hit_test(struct llk_ui_elem *elem);

/* 向 UI 元素及其子元素派发事件，执行事件回调 */
void llk__ui_elem_dispatch_event(struct llk_ui_elem *elem);

/* 绘制 UI 元素及其子元素 */
void llk__ui_elem_draw(struct llk_ui_elem *elem);

/* endregion */

/* region [callback] */

/* 根据 `key` 名字获取已注册的回调函数，获取不到则返回 `NULL` */
llk_ui_callback_fn llk__ui_get_callback(struct llk_ui_context *ui, const char *key);

/* endregion */

/* region [mem_alloc] */

/* 从 UI 上下文中的内存池中分配内存 */
void *llk__ui_mem_alloc(struct llk_ui_context *ui, size_t size);

/* 释放由 `llk__ui_mem_alloc()` 分配的内存 */
void llk__ui_mem_free(void *p);

/* 复制一份指定长度的字符串，存放在 UI 上下文中的内存池中 */
char *llk__ui_strdup(struct llk_ui_context *ui, const char *s, size_t len);

/* endregion */

/* region [parser] */

/* 将字符串解析成带单位的长度值
 *
 * 支持的单位 `px`、`%`、`vw`、`vh`，没有单位则默认单位为 `px`。
 * 函数不判断解析后的数字的取值范围是否合理（为负值，或超出范围）。
 *
 * 若解析成功，函数返回 0，否则返回非 0。
 */
int llk__ui_parse_length_val(const char *str, float *get_val, enum llk_ui_unit *get_unit);

/* 将字符串解析成带单位的颜色
 *
 * 支持的颜色格式：`#RRGGBB` 和 `#RRGGBBAA`，不区分字母大小写。
 * 若解析成功函数返回 0，否则返回非 0。
 */
int llk__ui_parse_color_val(const char *str, uint32_t *get_val, enum llk_ui_unit *get_unit);

/* endregion */

#endif
