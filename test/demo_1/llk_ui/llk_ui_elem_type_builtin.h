#ifndef LLK_UI_ELEM_TYPE_BUILTIN_H
#define LLK_UI_ELEM_TYPE_BUILTIN_H

#include "./llk_ui_ext.h"

/* llk UI 内置的元素类型
 *
 * 它们会在创建 UI 上下文的时候被注册。
 */

/* ------------------------------------------------------------------------ */

/* 根元素，类型名为 `root`
 *
 * 作为 UI 元素树的根节点，元素的宽高与 UI 画布的宽高保持一致。
 */
extern struct llk_ui_elem_type llk__ui_elem_root;

/* 普普通通的布局盒子，类型名为 `box`
 *
 * 支持的属性：
 *
 * - `background-color`         背景颜色（默认）
 * - `background-color.hovered` 背景颜色（鼠标悬浮时）
 * - `background-color.pressed` 背景颜色（鼠标按住时）
 *
 * - `border-color`         边框颜色（默认）
 * - `border-color.hovered` 边框颜色（鼠标悬浮时）
 * - `border-color.pressed` 边框颜色（鼠标按住时）
 *
 * 颜色格式：`#RRGGBB` 和 `#RRGGBBAA`，不区分字母大小写。
 *
 * - `on_click` 点击事件，鼠标点击元素时执行事件回调
 * - `on_draw`  绘制事件，交由你来绘制元素
 */
extern struct llk_ui_elem_type llk__ui_elem_box;

/* 图片，类型名为 `image`
 *
 * 支持的属性：
 * - `src` 用于指定要绘制的图片的名字，该名字由 `k_image_set_name()` 指定
 */
extern struct llk_ui_elem_type llk__ui_elem_image;

#endif
