#ifndef LLK_UI_H
#define LLK_UI_H

/** \brief UI 上下文 */
struct llk_ui_context;

/** \brief UI 元素 */
struct llk_ui_elem;

/* region [UI context] */

/** \brief 创建 UI 上下文 */
struct llk_ui_context *llk_ui_create_context(void);

/** \brief 销毁 UI 上下文。若 `ui` 为 `NULL` 则函数立即返回 */
void llk_ui_destroy_context(struct llk_ui_context *ui);

/* endregion */

/* region [build_ui] */

/** \brief 获取 UI 树的根节点，UI 树的构建必须从根节点开始 */
struct llk_ui_elem *llk_ui_get_root(struct llk_ui_context *ui);

/** \brief 创建一个 UI 元素 */
struct llk_ui_elem *llk_ui_create_elem(struct llk_ui_context *ui, const char *type_name);

/** \brief 设置 UI 元素的属性 */
int llk_ui_elem_set_attr(struct llk_ui_elem *elem, const char *key, const char *val);

/** \brief 添加 UI 子元素 */
int llk_ui_append_child(struct llk_ui_elem *parent, struct llk_ui_elem *child);

/* TODO */
void llk_ui_elem_remove(struct llk_ui_elem *elem);

/* endregion */

/* region [UI elem] */

/** \brief 检查鼠标指针是否悬浮在 UI 元素上 */
int llk_ui_elem_is_hovered(struct llk_ui_elem *elem);

/** \brief 检查鼠标是按在 UI 元素上 */
int llk_ui_elem_is_pressed(struct llk_ui_elem *elem);

/* endregion */

/* region [find_elem] */

/** \brief 获取指定 id 的 UI 元素 */
struct llk_ui_elem *llk_ui_get_elem_by_id(struct llk_ui_context *ui, const char *id);

/* endregion */

/* region [parse_from_xml] */

/** \brief 解析 xml 文件，生成对应的 UI 元素 */
struct llk_ui_elem *llk_ui_build_elem_from_xml_file(struct llk_ui_context *ui, const char *file_path);

/* endregion */

/* region [draw] */

/** \brief 更新 UI 状态，处理输入事件，计算元素尺寸，定位元素位置 */
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

#endif
