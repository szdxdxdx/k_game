#ifndef LLK_UI_H
#define LLK_UI_H

/** \brief UI 上下文 */
struct llk_ui_context;

/** \brief UI 元素 */
struct llk_ui_elem;

/* region [UI context] */

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

/** \brief 事件回调函数 */
typedef void (*llk_ui_callback_fn)(struct llk_ui_elem *elem);

/**
 * \brief 注册事件回调函数
 *
 * UI 元素通过 `key` 名字来绑定回调，而不是函数指针
 *
 * 注册回调函数，需要提交函数指针，以及唯一的 `key` 作为它的名字，
 * 若注册成功，函数返回 0，否则返回非 0。
 */
int llk_ui_register_callback(struct llk_ui_context *ui, const char *key, llk_ui_callback_fn fn_callback);

/* endregion */

/* region [build_ui] */

/** \brief 获取 UI 树的根节点，UI 树的构建必须从根节点开始 */
struct llk_ui_elem *llk_ui_get_root(struct llk_ui_context *ui);

/** \brief 添加 UI 子元素 */
int llk_ui_append_child(struct llk_ui_elem *parent, struct llk_ui_elem *child);

/** \brief 解析 xml 文件，构建对应的 UI 元素 */
struct llk_ui_elem *llk_ui_build_elem_from_xml_file(struct llk_ui_context *ui, const char *file_path);

/* endregion */

/* region [find_elem] */

/** \brief 根据 id 查找 UI 元素，找不到则返回 `NULL` */
struct llk_ui_elem *llk_ui_get_elem_by_id(struct llk_ui_context *ui, const char *id);

/* endregion */

/* endregion */

/* region [UI elem] */

/* region [create] */

/** \brief 创建一个 UI 元素 */
struct llk_ui_elem *llk_ui_create_elem(struct llk_ui_context *ui, const char *type_name);

/* TODO */
void llk_ui_destroy_elem(struct llk_ui_elem *elem);

/* endregion */

/* region [set_attr] */

/**
 * \brief 设置 UI 元素的属性
 *
 * 所有元素都支持的属性：
 * - `id`
 * - `w, h`
 * - `left, right, top, bottom`
 * - `display`
 */
int llk_ui_elem_set_attr(struct llk_ui_elem *elem, const char *key, const char *val);

/* endregion */

/* region [get] */

/** \brief 检查鼠标指针是否悬浮在该 UI 元素上，若是，函数返回非 0，否则返回 0*/
int llk_ui_elem_is_hovered(struct llk_ui_elem *elem);

/** \brief 检查鼠标点击了该 UI 元素，若是，函数返回非 0，否则返回 0 */
int llk_ui_elem_is_clicked(struct llk_ui_elem *elem);

/** \brief 检查鼠标是否按在该 UI 元素上，若是，函数返回非 0，否则返回 0 */
int llk_ui_elem_is_pressed(struct llk_ui_elem *elem);

/** \brief 获取 UI 元素矩形左上角的在 UI 界面中的坐标以及矩形的宽高 */
void llk_ui_elem_get_rect(struct llk_ui_elem *elem, float *get_x, float *get_y, float *get_w, float *get_h);

/* endregion */

/* endregion */

#endif
