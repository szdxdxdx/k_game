#ifndef K_CALLBACK_H
#define K_CALLBACK_H

#include "./k_game_fwd.h"

/**
 * \brief 添加事件回调
 *
 * k_game 中，通过 `k_XXX_add_YYY_callback()` 的方式添加事件回调。
 * XXX 表示挂载回调的载体（或称宿主），可以是房间、对象、组件、组件管理器。
 * YYY 表示事件的类型，表示的是触发回调的时机或条件，即什么时候或在什么情况下触发回调。
 *
 * @{
 */

struct k_callback *k_room_add_begin_step_callback(void *data, void (*fn_callback)(void *data));
struct k_callback *k_room_add_alarm_callback     (void *data, void (*fn_callback)(void *data, int timeout_diff), int delay_ms);
struct k_callback *k_room_add_step_callback      (void *data, void (*fn_callback)(void *data));
struct k_callback *k_room_add_end_step_callback  (void *data, void (*fn_callback)(void *data));
struct k_callback *k_room_add_draw_callback      (void *data, void (*fn_callback)(void *data), int z_group, int z_layer);

struct k_callback *k_object_add_destroy_callback   (struct k_object *object, void (*fn_callback)(struct k_object *object));
struct k_callback *k_object_add_begin_step_callback(struct k_object *object, void (*fn_callback)(struct k_object *object));
struct k_callback *k_object_add_alarm_callback     (struct k_object *object, void (*fn_callback)(struct k_object *object, int timeout_diff), int delay_ms);
struct k_callback *k_object_add_step_callback      (struct k_object *object, void (*fn_callback)(struct k_object *object));
struct k_callback *k_object_add_end_step_callback  (struct k_object *object, void (*fn_callback)(struct k_object *object));
struct k_callback *k_object_add_draw_callback      (struct k_object *object, void (*fn_callback)(struct k_object *object), int z_group, int z_layer);

struct k_callback *k_component_add_begin_step_callback(struct k_component *component, void (*fn_callback)(struct k_component *component));
struct k_callback *k_component_add_alarm_callback     (struct k_component *component, void (*fn_callback)(struct k_component *component, int timeout_diff), int delay_ms);
struct k_callback *k_component_add_step_callback      (struct k_component *component, void (*fn_callback)(struct k_component *component));
struct k_callback *k_component_add_end_step_callback  (struct k_component *component, void (*fn_callback)(struct k_component *component));
struct k_callback *k_component_add_draw_callback      (struct k_component *component, void (*fn_callback)(struct k_component *component), int z_group, int z_layer);

struct k_callback *k_component_manager_add_begin_step_callback(struct k_component_manager *manager, void *data, void (*fn_callback)(void *data));
struct k_callback *k_component_manager_add_alarm_callback     (struct k_component_manager *manager, void *data, void (*fn_callback)(void *data, int timeout_diff), int delay_ms);
struct k_callback *k_component_manager_add_step_callback      (struct k_component_manager *manager, void *data, void (*fn_callback)(void *data));
struct k_callback *k_component_manager_add_end_step_callback  (struct k_component_manager *manager, void *data, void (*fn_callback)(void *data));
struct k_callback *k_component_manager_add_draw_callback      (struct k_component_manager *manager, void *data, void (*fn_callback)(void *data), int z_group, int z_layer);

/** @} */

/**
 * \brief 删除事件回调
 *
 * 销毁房间、对象、组件、组件管理器时，其挂载的事件回调也会被一并移除，无需手动移除。
 * 若 `callback` 为 `NULL`，则函数立即返回。
 */
void k_callback_del(struct k_callback *callback);

#endif
