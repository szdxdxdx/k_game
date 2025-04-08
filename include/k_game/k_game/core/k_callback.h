#ifndef K_CALLBACK_H
#define K_CALLBACK_H

struct k_room;
struct k_object;
struct k_component;

/**
 * \brief
 *
 */
struct k_callback;

/* region [room_callback] */

struct k_callback *k_room_add_step_begin_callback(void *data, void (*fn_callback)(void *data));

struct k_callback *k_room_add_alarm_callback(void *data, void (*fn_callback)(void *data, int timeout_diff), int delay_ms);

struct k_callback *k_room_add_step_callback(void *data, void (*fn_callback)(void *data));

struct k_callback *k_room_add_step_end_callback(void *data, void (*fn_callback)(void *data));

struct k_callback *k_room_add_draw_callback(void *data, void (*fn_callback)(void *data), int z_group, int z_layer);

/**
 * \brief 删除房间的事件回调
 *
 * 若 `callback` 为 `NULL`，则函数立即返回。
 */
void k_room_del_callback(struct k_callback *room_callback);

/* endregion */

/* region [object_callback] */

/**
 * \brief 对象回调
 *
 * TODO docs
 */
struct k_callback;

struct k_callback *k_object_add_step_begin_callback(struct k_object *object, void (*fn_callback)(struct k_object *object));

struct k_callback *k_object_add_alarm_callback(struct k_object *object, void (*fn_callback)(struct k_object *object, int timeout_diff), int delay_ms);

struct k_callback *k_object_add_step_callback(struct k_object *object, void (*fn_callback)(struct k_object *object));

struct k_callback *k_object_add_step_end_callback(struct k_object *object, void (*fn_callback)(struct k_object *object));

struct k_callback *k_object_add_draw_callback(struct k_object *object, void (*fn_callback)(struct k_object *object), int z_group, int z_layer);

void k_object_del_callback(struct k_callback *object_callback);

/* endregion */

/* region [component_callback] */

struct k_callback *k_component_add_step_begin_callback(struct k_component *component, void (*fn_callback)(struct k_component *component));

struct k_callback *k_component_add_alarm_callback(struct k_component *component, void (*fn_callback)(struct k_component *component, int timeout_diff), int delay_ms);

struct k_callback *k_component_add_step_callback(struct k_component *component, void (*fn_callback)(struct k_component *component));

struct k_callback *k_component_add_step_end_callback(struct k_component *component, void (*fn_callback)(struct k_component *component));

struct k_callback *k_component_add_draw_callback(struct k_component *component, void (*fn_callback)(struct k_component *component), int z_group, int z_layer);

void k_component_del_callback(struct k_callback *component_callback);

/* endregion */

#endif
