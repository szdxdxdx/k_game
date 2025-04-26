#ifndef LLK_UI_H
#define LLK_UI_H

/** \brief UI 上下文 */
struct llk_ui_context;

/** \brief UI 元素 */
struct llk_ui_elem;

/* region [UI_context] */

/* region [create] */

/** \brief 创建 UI 上下文 */
struct llk_ui_context *llk_ui_create_context(void);

/** \brief 销毁 UI 上下文。若 `ui` 为 `NULL` 则函数立即返回 */
void llk_ui_destroy_context(struct llk_ui_context *ui);

/* endregion */

/* region [update && draw] */

/** \brief 更新 UI 状态，计算元素尺寸，定位元素位置，触发事件回调 */
void llk_ui_update(struct llk_ui_context *ui);

/** \brief 绘制 UI 界面 */
void llk_ui_draw(struct llk_ui_context *ui);

/* endregion */

/* region [register_callback] */

/**
 * \brief 注册事件回调函数
 *
 * UI 元素通过 `key` 名字来绑定回调，而不是函数指针
 *
 * 注册回调函数，需要提交函数指针 `fn_callback`，以及唯一的 `key` 作为它的名字。
 *
 * 由于不同 UI 元素的回调的入参不同，所以此处使用 `void *` 来接收所有类型的函数指针。
 * 回调触发时，会根据 UI 元素的类型对该指针进行类型转换并调用。
 * 请确保注册函数的签名与 UI 元素所需的回调类型一致。
 *
 * 若注册成功，函数返回 0，否则返回非 0。
 */
int llk_ui_register_callback(struct llk_ui_context *ui, const char *key, void *fn_callback);

/* endregion */

/* region [build_ui] */

/** \brief 获取 UI 树的根节点，UI 树的构建必须从根节点开始 */
struct llk_ui_elem *llk_ui_get_root(struct llk_ui_context *ui);

/** \brief 添加 UI 子元素 */
int llk_ui_elem_append_child(struct llk_ui_elem *parent, struct llk_ui_elem *child);

/** \brief 解析 xml 文件，构建对应的 UI 元素 */
struct llk_ui_elem *llk_ui_build_elem_from_xml_file(struct llk_ui_context *ui, const char *file_path);

/* endregion */

/* region [find_elem] */

/** \brief 根据 id 查找 UI 元素，找不到则返回 `NULL` */
struct llk_ui_elem *llk_ui_get_elem_by_id(struct llk_ui_context *ui, const char *id);

/* endregion */

/* endregion */

/* region [UI_elem] */

/* region [create] */

/** \brief 创建 UI 元素 */
struct llk_ui_elem *llk_ui_elem_create(struct llk_ui_context *ui, const char *type_name);

/** \brief 销毁 UI 元素，将其从 UI 树中摘除 */
void llk_ui_elem_destroy(struct llk_ui_elem *elem);

/* endregion */

/* region [set_attr] */

/**
 * \brief 设置 UI 元素的属性
 *
 * 所有元素都支持的属性：
 *
 * - `id`
 *   元素的唯一标识符。元素默认没有 id，你可以为元素指定唯一的 id，用来方便查找元素。
 *   设置 id 时，若已有其他元素使用相同的 id，将设置失败。
 *
 * - `w, h`
 *   用于指定元素的宽和高。可选的单位有：`px`、`%`、`vw`、`vh`：
 *   - `px` 像素
 *   - `%`  相对父元素的百分比
 *   - `vw` 视口宽度的百分比
 *   - `vh` 视口高度的百分比
 *   - 若没有指定单位，则默认单位是 `px`
 *
 * - `left, right, top, bottom`
 *   用于指定元素的边相对于父元素的偏移。
 *   可选的单位有：`px`、`%`、`vw`、`vh`，若没有带单位，则默认单位为像素。
 *   若元素的 `w` 已被指定，且同时指定 `left` 和 `right`，则 `right` 的值将被忽略，
 *   若元素的 `w` 未指定，且同时指定 `left` 和 `right`，则拉伸宽度以适应偏移。高度同理。
 *
 * - `hidden`
 *   控制元素是否被隐藏。可选取值只有 `true` 和 `false`。
 *   若为 `true`，则该元素及其子元素不可见，不参与布局、事件处理和绘制。
 */
int llk_ui_elem_set_attr(struct llk_ui_elem *elem, const char *key, const char *val);

/* endregion */

/* region [get] */

/** \brief 若当前帧内鼠标指针是否悬浮在该 UI 元素上，则函数返回非 0，否则返回 0 */
int llk_ui_elem_is_hovered(struct llk_ui_elem *elem);

/** \brief 若当前帧内鼠标点击了该 UI 元素，则函数返回非 0，否则返回 0 */
int llk_ui_elem_is_clicked(struct llk_ui_elem *elem);

/** \brief 若当前帧内鼠标是否按在该 UI 元素上，则函数返回非 0，否则返回 0 */
int llk_ui_elem_is_pressed(struct llk_ui_elem *elem);

/** \brief 获取 UI 元素矩形左上角的在 UI 界面中的坐标以及矩形的宽高 */
void llk_ui_elem_get_rect(struct llk_ui_elem *elem, float *get_x, float *get_y, float *get_w, float *get_h);

/* endregion */

/* endregion */

#endif
