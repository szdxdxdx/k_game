#ifndef LLK_UI_EXT_H
#define LLK_UI_EXT_H

#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "k_mem_pool.h"
#include "k_intrusive_map.h"
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
     * 例如：`50.0%`、`100%`。字符串值的取值为 `100%` 时，
     * 对应解析后的指定的值为 1.0f，计算后的值的单位为：像素。
     */
    LLK_UI_UNIT_PERCENT,

    /* 相对于视窗的宽高比
     *
     * 例如： `50vw`、`100vh`。字符串值的取值为 `100vw` 或 `100vh` 时，
     * 对应解析后的指定的值为 1.0f，计算后的值的单位为：像素。
     */
    LLK_UI_UNIT_VW,
    LLK_UI_UNIT_VH,

    /* 十六进制颜色值
     *
     * 例如：
     * - 不带透明度 RGB `#ff6600`
     * - 带透明度 RGBA `#ee000066`
     *
     * 计算后的值为 uint32_t 类型的 0xRRGGBBAA 十六进制颜色值。
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

    /* 鼠标在 UI 界面中的坐标 */
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

/* 若布局脏标记为非 0，则重新布局 */
void llk__ui_layout_if_dirty(struct llk_ui_context *ui);

/* endregion */

/* region [UI_elem_type] */

struct llk_ui_elem_type;

/* 注册自定义的 UI 元素类型
 *
 * 只有注册类型成功后，才能通过 `llk_ui_elem_create()` 创建该类型的 UI 元素实例。
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
    size_t elem_struct_size;

    /* 初始化函数，在创建实例时调用
     *
     * 若初始化成功，函数应返回 0，否则应返回非 0。
     */
    int (*on_create)(struct llk_ui_elem *elem);

    /* 析构函数，在销毁实例时调用 */
    void (*on_destroy)(struct llk_ui_elem *elem);

    /* 设置属性的函数，在设置属性时调用
     *
     * 你需要先自己解析 key，来判断要设置的是否是该类型的属性，
     * 若是，你需要自己解析 val 来设置值；若设置成功，函数应返回 0，否则应返回非 0。
     *
     * 若不是该类型的属性，你应调用 `llk__ui_elem_set_attr_default()` 交由基类处理。
     */
    int (*fn_set_attr)(struct llk_ui_elem *elem, const char *key, const char *val);

    /* 完成该元素的布局后执行的回调
     *
     * llk UI 将从根节点开始，逐层向下计算 UI 元素的宽高尺寸（measure），
     * 然后定位坐标位置（layout），在完成元素（及其子元素）的布局后会执行该回调。
     * 此时你可以获取计算后的元素的坐标 `elem->x, elem->y`
     * 以及宽高 `elem->w.computed_val, elem->h.computed_val`。
     */
    void (*fn_after_layout)(struct llk_ui_elem *elem);

    /* 完成该元素的命中测试后执行的回调
     *
     * llk UI 将从根节点开始，逐层向下判断鼠标是否名字该元素，
     * 也就是鼠标的指针是否悬浮在该元素上，判断完成后执行此回调。
     * 你可以通过 `elem->is_hovered` 判断是否命中，
     * 并结合 `elem->ui->mouse_button_down` 判断是否点击了该元素。
     */
    void (*fn_hit_test)(struct llk_ui_elem *elem);

    /* 事件分发阶段要执行的回调
     *
     * 此时元素的布局和命中测试也已完成，你可以在这个阶段触发你定义的事件回调。
     * 你需要设计该类型的元素支持什么样的回调，然后在事件分发阶段触发它们。
     *
     * 事件分发的顺序是，先触发子元素的事件回调，再轮到父元素。
     * 你不需要主动通知父元素触发事件回调。
     */
    void (*fn_dispatch_event)(struct llk_ui_elem *elem);

    /* 绘制阶段执行的回调
     *
     * 完全交由你来绘制元素，请将元素绘制在合理的地方。
     * 若设为 `NULL` 则忽略绘制，表明该元素仅用于定位布局。
     *
     * 绘制的顺序是，先绘制父元素，再绘制子元素。
     * 你不需要主动通知子元素执行绘制。
     */
    void (*fn_draw)(struct llk_ui_elem *elem);
};

/* endregion */

/* region [UI_elem] */

/* UI 元素 */
struct llk_ui_elem {

    /* UI 元素所属的上下文 */
    struct llk_ui_context *ui;

    /* 父元素 */
    struct llk_ui_elem *parent;

    /* 子元素 */
    struct k_list child_list;

    /* 指向相邻兄弟元素的指针域 */
    struct k_list_node sibling_link;

    /* 该元素的类型信息 */
    const struct llk_ui_elem_type *type;

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

    /* 标记鼠标指针是否悬浮在该元素上（计算布局后得出） */
    unsigned int is_hovered;

    /* 标记该元素（及其子元素）是否显示 */
    unsigned int is_hidden;

    /* 销毁元素时将它标记为逻辑删除，延迟销毁 */
    unsigned int flag_destroy;
};

/* 销毁 UI 元素 */
void llk__ui_elem_destroy(struct llk_ui_elem *elem);

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
void *llk__ui_get_callback(struct llk_ui_context *ui, const char *key);

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

/* region [UI_elem_builtin] */

/**
 * \brief 普普通通的布局盒子
 *
 * 元素类型名为 `box`。
 *
 * # `llk_ui_elem_set()` 支持的属性：
 *
 * ## 背景颜色
 * - `background-color`         默认
 * - `background-color.hovered` 鼠标悬浮时
 * - `background-color.pressed` 鼠标按住时
 * 示例：
 * ```C
 * llk_ui_elem_set(elem, "background-color", "#ee0000dd")
 * llk_ui_elem_set(elem, "background-color.hovered", "#ee0000")
 * ```
 *
 * ## 边框颜色
 * - `border-color`         默认
 * - `border-color.hovered` 鼠标悬浮时
 * - `border-color.pressed` 鼠标按住时
 *
 * ## 事件回调
 * - `on-click` 鼠标点击元素时执行该事件回调
 * 示例
 * ```C
 * void on_click(struct llk_ui_elem *elem) {
 *     struct llk_ui_elem_box *box = (struct llk_ui_elem_box *)elem;
 *     // ...
 *     puts("clicked");
 * }
 *
 * // 注册回调
 * llk_ui_register_callback(ui, "my_click_fn", (void *)on_click);
 *
 * // 绑定回调
 * llk_ui_elem_set(elem, "on-click", "my_click_fn")
 * ```
 *
 * ## 绘制 `on-draw`
 * 元素绘制完自身后，交由你来补充绘制内容，
 * 你可以自由绘制任何内容，绘制到你认为合适的地方。
 * 示例：
 * ```C
 * void on_draw(struct llk_ui_elem *elem) {
 *     float x, y, w, h;
 *     llk_ui_elem_get_rect(elem, &x, &y, &w, &h);
 *     k_canvas_set_draw_color(0xff6600ff)
 *     k_canvas_ui_fill_rect(x, y, w, h);
 * }
 * ```
 */
struct llk_ui_elem_box {
    struct llk_ui_elem super;

    struct llk_ui_u32 background_color;
    struct llk_ui_u32 background_color_hovered;
    struct llk_ui_u32 background_color_pressed;

    struct llk_ui_u32 border_color;
    struct llk_ui_u32 border_color_hovered;
    struct llk_ui_u32 border_color_pressed;

    void (*fn_on_click)(struct llk_ui_elem *elem);
    void (*fn_on_draw)(struct llk_ui_elem *elem);
};

/**
 * \brief 滑动条
 *
 * 元素类型名为 `slider`。
 *
 * # `llk_ui_elem_set()` 支持的属性：
 *
 * ## 滑槽颜色
 * - `track-color`         默认
 * - `track-color.hovered` 鼠标悬浮时
 * - `track-color.pressed` 鼠标按住时
 *
 * ## 滑块颜色
 * - `thumb-color`         默认
 * - `thumb-color.hovered` 鼠标悬浮时
 * - `thumb-color.pressed` 鼠标按住时
 *
 * ## 边框颜色
 * - `border-color`         默认
 * - `border-color.hovered` 鼠标悬浮时
 * - `border-color.pressed` 鼠标按住时
 *
 * ## 取值范围、步长
 * - `min`  最小值
 * - `max`  最大值
 * - `val`  当前值
 * - `step` 步长
 *
 * ## 事件回调
 * - `on-change` 滑动条的值发生变化时要执行的回调
 * 示例
 * ```C
 * void on_change(struct llk_ui_elem *elem, float old_val, float new_val) {
 *     struct llk_ui_elem_slider *slider = (struct llk_ui_elem_slider *)elem;
 *     // ...
 *     printf("old_val: %.2f, new_val: %.2f", old_val, new_val);
 * }
 */
struct llk_ui_elem_slider {
    struct llk_ui_elem super;

    /* 滑槽颜色 */
    struct llk_ui_u32 track_color;
    struct llk_ui_u32 track_color_hovered;
    struct llk_ui_u32 track_color_pressed;

    /* 滑块颜色 */
    struct llk_ui_u32 thumb_color;
    struct llk_ui_u32 thumb_color_hovered;
    struct llk_ui_u32 thumb_color_pressed;

    /* 边框颜色 */
    struct llk_ui_u32 border_color;
    struct llk_ui_u32 border_color_hovered;
    struct llk_ui_u32 border_color_pressed;

    /* 滑动条的取值范围、步长 */
    float min;
    float max;
    float step;

    /* 当前值、旧值 */
    float val;
    float old_val;

    /* 当前鼠标是否在移动滑块 */
    unsigned int is_active;

    /* 值是否改变 */
    unsigned int is_changed;

    /* 值发生变化后执行的回调 */
    void (*fn_on_change)(struct llk_ui_elem *elem, float old_val, float new_val);
};

/* 图片，类型名为 `image`
 *
 * 支持的属性：
 * - `src` 用于指定要绘制的图片的名字，该名字由 `k_image_set_name()` 指定
 */
struct llk_ui_elem_image {
    struct llk_ui_elem super;

    struct k_image *src;
};

/* endregion */

#endif
