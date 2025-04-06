#ifndef K_OBJECT_H
#define K_OBJECT_H

#include <stddef.h>

struct k_room;

/**
 * \brief 对象
 *
 * 对象是游戏中会发生动作的实体。它们有行为，例如：移动、攻击等。
 * 大多数情况下，对象会关联精灵作为自己的外观，使得它们能被看见。
 * 你在游戏中看到的绝大多数东西都是对象，例如：主角、子弹、墙壁等。
 * 当然也有一些对象没有外观，它们仅用于控制游戏的运作。
 */
struct k_object;

/* region [object_create] */

struct k_object *k_object_create(size_t data_size);

void k_object_destroy(struct k_object *object);

/* endregion */

/* region [object_get/set] */

void *k_object_get_data(struct k_object *object);

void k_object_set_destroy_callback(struct k_object *object, void (*fn_callback)(struct k_object *object));

/* endregion */

/* region [object_add_callback] */

/**
 * \brief 对象回调
 *
 * TODO docs
 */
struct k_object_callback;

struct k_object_callback *k_object_add_step_begin_callback(struct k_object *object, void (*fn_callback)(struct k_object *object));

struct k_object_callback *k_object_add_alarm_callback(struct k_object *object, void (*fn_callback)(struct k_object *object, int timeout_diff), int delay_ms);

struct k_object_callback *k_object_add_step_callback(struct k_object *object, void (*fn_callback)(struct k_object *object));

struct k_object_callback *k_object_add_step_end_callback(struct k_object *object, void (*fn_callback)(struct k_object *object));

struct k_object_callback *k_object_add_draw_callback(struct k_object *object, void (*fn_callback)(struct k_object *object), int z_group, int z_layer);

void k_object_del_callback(struct k_object_callback *callback);

void k_object_del_all_callbacks(struct k_object *object);

/* endregion */

#endif
