#ifndef K_GAME_COMPONENT_H
#define K_GAME_COMPONENT_H

#include <stddef.h>

struct k_object;

/**
 * \brief 组件类型
 *
 * TODO docs
 */
struct k_component_type;

/**
 * \brief 组件
 *
 * TODO docs
 */
struct k_component;

/* region [define_component_type] */

struct k_component_type_config {

    const char *type_name;

    size_t data_size;

    int (*fn_init)(struct k_component *component, void *params);

    void (*fn_fini)(struct k_component *component);
};

#define K_COMPONENT_TYPE_CONFIG_INIT \
{ \
    .type_name = NULL, \
    .data_size = 0,    \
    .fn_init   = NULL, \
    .fn_fini   = NULL  \
}

struct k_component_type *k_define_component_type(const struct k_component_type_config *config);

/* endregion */

/* region [component_type_get] */

const char *k_component_type_get_name(struct k_component_type *component_type);

struct k_component_type *k_get_component_type_by_name(const char *type_name);

/* endregion */

/**
 * \brief 组件回调
 *
 * TODO docs
 */
struct k_component_callback;

/* region [object_add_component] */

/*
 * TODO docs
 *
 * 往对象上挂载组件时，会根据给定的组件类型创造一个组件实例。
 * 组件不能独立存在，必须要挂载到对象上。
 * 当对象被销毁时，对象所携带的组件也会跟着一并销毁。
 */
struct k_component *k_object_add_component(struct k_object *object, struct k_component_type *component_type, void *params);

void k_object_del_component(struct k_component *component);

// TODO struct k_component *k_object_get_component(struct k_object *object, struct k_component_type *component_type);

// TODO int k_get_objects_with_component(size_t component_type_id, struct k_object ***get_objects, size_t *n);

/* endregion */

/* region [component_callback] */

struct k_component_callback *k_component_add_alarm_callback(struct k_component *component, void (*fn_callback)(struct k_component *component, int timeout_diff), int delay_ms);

struct k_component_callback *k_component_add_step_callback(struct k_component *component, void (*fn_callback)(struct k_component *component));

struct k_component_callback *k_component_add_draw_callback(struct k_component *component, void (*fn_callback)(struct k_component *component), int z_index);

void k_component_del_callback(struct k_component_callback *callback);

/* endregion */

/* region [component_get] */

void *k_component_get_data(struct k_component *component);

struct k_object *k_component_get_object(struct k_component *component);

/* endregion */

#endif
