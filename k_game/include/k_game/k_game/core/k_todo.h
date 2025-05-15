#ifndef K_TODO_H
#define K_TODO_H

#include <stddef.h>

#include "./k_game_fwd.h"

/* TODO 该头文件下均是【可能】【将要实现】的 api */

/* 若 `target` 被销毁，则 `object` 也跟着被销毁 */
int k_object_set_follow_destruction(struct k_object *object, struct k_object *target);

/* 遍历对象所挂载的组件，查找并返回指定组件类型的组件实例（有多个则返回找到的第一个） */
struct k_component *k_object_find_component(struct k_object *object, struct k_component_type *component_type);

/* region [id] */

/* 引入 ID 可以避免悬空指针问题 */

size_t k_object_get_id(struct k_object *object);

struct k_object *k_object_find_by_id(size_t id);

/* endregion */

/* region [event_callback] */

struct k_event_callback;

struct k_event_callback_v_tbl {
    struct k_event_callback *(*fn_add)(void *param);
    void (*fn_del)(struct k_event_callback *callback);
};

struct k_event_callback {
    struct k_event_callback_v_tbl v_tbl;
};

struct k_event_callback *k_object_add_destroy_event   (struct k_object *object, void (*fn_callback)(struct k_object *object));
struct k_event_callback *k_object_add_begin_step_event(struct k_object *object, void (*fn_callback)(struct k_object *object));
struct k_event_callback *k_object_add_alarm_event     (struct k_object *object, void (*fn_callback)(struct k_object *object));
struct k_event_callback *k_object_add_step_event      (struct k_object *object, void (*fn_callback)(struct k_object *object));
struct k_event_callback *k_object_add_end_step_event  (struct k_object *object, void (*fn_callback)(struct k_object *object));
struct k_event_callback *k_object_add_draw_event      (struct k_object *object, void (*fn_callback)(struct k_object *object));
struct k_event_callback *k_object_add_enter_room_event(struct k_object *object, void (*fn_callback)(struct k_object *object));
struct k_event_callback *k_object_add_leave_room_event(struct k_object *object, void (*fn_callback)(struct k_object *object));

/* endregion */

#endif
