#ifndef K_CALLBACK_H
#define K_CALLBACK_H

struct k_room;
struct k_object;
struct k_component;
struct k_component_manager;

/**
 * \brief
 *
 */
struct k_callback;

/**
 * \brief 删除事件回调
 *
 * 若 `callback` 为 `NULL`，则函数立即返回。
 */
void k_callback_del(struct k_callback *callback);

/* region [room_add_callback] */

struct k_callback *k_room_add_step_begin_callback(void *data, void (*fn_callback)(void *data));

struct k_callback *k_room_add_alarm_callback(void *data, void (*fn_callback)(void *data, int timeout_diff), int delay_ms);

struct k_callback *k_room_add_step_callback(void *data, void (*fn_callback)(void *data));

struct k_callback *k_room_add_step_end_callback(void *data, void (*fn_callback)(void *data));

struct k_callback *k_room_add_draw_callback(void *data, void (*fn_callback)(void *data), int z_group, int z_layer);

/* endregion */

/* region [object_add_callback] */

struct k_callback *k_object_add_step_begin_callback(struct k_object *object, void (*fn_callback)(struct k_object *object));

struct k_callback *k_object_add_alarm_callback(struct k_object *object, void (*fn_callback)(struct k_object *object, int timeout_diff), int delay_ms);

struct k_callback *k_object_add_step_callback(struct k_object *object, void (*fn_callback)(struct k_object *object));

struct k_callback *k_object_add_step_end_callback(struct k_object *object, void (*fn_callback)(struct k_object *object));

struct k_callback *k_object_add_draw_callback(struct k_object *object, void (*fn_callback)(struct k_object *object), int z_group, int z_layer);

/* endregion */

/* region [component_add_callback] */

struct k_callback *k_component_add_step_begin_callback(struct k_component *component, void (*fn_callback)(struct k_component *component));

struct k_callback *k_component_add_alarm_callback(struct k_component *component, void (*fn_callback)(struct k_component *component, int timeout_diff), int delay_ms);

struct k_callback *k_component_add_step_callback(struct k_component *component, void (*fn_callback)(struct k_component *component));

struct k_callback *k_component_add_step_end_callback(struct k_component *component, void (*fn_callback)(struct k_component *component));

struct k_callback *k_component_add_draw_callback(struct k_component *component, void (*fn_callback)(struct k_component *component), int z_group, int z_layer);

/* endregion */

/* region [room_add_callback] */

struct k_callback *k_component_manager_add_step_begin_callback(struct k_component_manager *manager, void *data, void (*fn_callback)(void *data));

struct k_callback *k_component_manager_add_alarm_callback(struct k_component_manager *manager, void *data, void (*fn_callback)(void *data, int timeout_diff), int delay_ms);

struct k_callback *k_component_manager_add_step_callback(struct k_component_manager *manager, void *data, void (*fn_callback)(void *data));

struct k_callback *k_component_manager_add_step_end_callback(struct k_component_manager *manager, void *data, void (*fn_callback)(void *data));

struct k_callback *k_component_manager_add_draw_callback(struct k_component_manager *manager, void *data, void (*fn_callback)(void *data), int z_group, int z_layer);

/* endregion */

#endif
