#ifndef LLK_UI_H
#define LLK_UI_H

/** \brief UI 上下文 */
struct llk_ui_context;

/** \brief UI 元素 */
struct llk_ui_elem;

/* region [create_context] */

/** \brief 创建 UI 上下文 */
struct llk_ui_context *llk_ui_create_context(void);

/** \brief 销毁 UI 上下文 */
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

/* endregion */

/* region [build_ui_from_xml] */

/** \brief 解析 xml 文件，生成对应的 UI 上下文 */
struct llk_ui_context *llk_ui_build_context_from_xml_file(const char *file_path);

/* endregion */

/* region [draw] */

/** \brief 更新 UI 状态，处理输入事件，计算元素尺寸，定位元素位置 */
void llk_ui_update(struct llk_ui_context *ui);

/** \brief 绘制 UI 界面 */
void llk_ui_draw(struct llk_ui_context *ui);

/* endregion */

#endif
