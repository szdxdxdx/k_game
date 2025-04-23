#ifndef LLK_UI_ELEM_TYPE_BUILTIN_H
#define LLK_UI_ELEM_TYPE_BUILTIN_H

#include "./llk_ui_ext.h"

/* llk UI 内建的 UI 元素 */

/* 根元素，类型名为 `root`
 *
 * 作为 UI 元素树的根节点，元素的宽高与 UI 画布的宽高保持一致。
 */
extern struct llk_ui_elem_type llk__ui_elem_root;

/* 普通的布局盒子，类型名为 `box`
 *
 * 支持的属性：
 * - `background-color` 背景颜色
 * - `border-color` 边框颜色
 */
extern struct llk_ui_elem_type llk__ui_elem_box;

/* 图片，类型名为 `image`
 *
 * 支持的属性：
 * - `src` 用于指定要绘制的图片的名字，该名字由 `k_image_set_name()` 指定
 */
extern struct llk_ui_elem_type llk__ui_elem_image;

#endif
