#ifndef K_TODO_H
#define K_TODO_H

#include <stddef.h>

#include "./k_game_fwd.h"

/* TODO 该头文件下均是【可能】【将要实现】的 api */

/* 若 `target` 被销毁，则 `object` 也跟着被销毁 */
int k_object_set_follow_destruction(struct k_object *object, struct k_object *target);

/* 遍历对象所挂载的组件，查找并返回指定组件类型的组件实例（有多个则返回找到的第一个） */
struct k_component *k_object_find_component(struct k_object *object, struct k_component_type *component_type);

#endif
