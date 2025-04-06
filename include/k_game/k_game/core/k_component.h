#ifndef K_COMPONENT_H
#define K_COMPONENT_H

#include <stddef.h>

struct k_room;
struct k_object;

/**
 * \brief 组件
 * TODO docs
 */
struct k_component;

/**
 * \brief 组件管理器
 * TODO docs
 */
struct k_component_manager;

/**
 * \brief 组件类型
 * TODO docs
 */
struct k_component_type;

/* region [component_define] */

struct k_component_entity_config {

    size_t data_size;

    int (*fn_init)(struct k_component *component, void *params);

    void (*fn_fini)(struct k_component *component);

    int (*fn_command)(struct k_component *component, char *get_result, size_t n, const char *command);
};

#define K_COMPONENT_ENTITY_CONFIG_INIT \
{ \
    .data_size = 0,    \
    .fn_init   = NULL, \
    .fn_fini   = NULL  \
}

struct k_component_manager_config {

    size_t data_size;

    int (*fn_init)(struct k_component_manager *manager, void *params);

    void (*fn_fini)(struct k_component_manager *manager);
};

#define K_COMPONENT_MANAGER_CONFIG_INIT \
{ \
    .data_size = 0,    \
    .fn_init   = NULL, \
    .fn_fini   = NULL  \
}

struct k_component_type *k_component_define(const struct k_component_manager_config *manager_config, const struct k_component_entity_config *entity_config);

/* endregion */

/* region [component_type_find] */

int k_component_type_set_name(struct k_component_type *component_type, const char *type_name);

struct k_component_type *k_component_type_find(const char *type_name);

/* endregion */

/* region [object_add_component] */

/**
 * \brief 创建一个组件实例，并挂载到指定的对象上
 *
 * 函数将入参 `params` 转交给组件的初始化回调 `fn_init()`。
 *
 * 若创建成功，函数返回组件指针，否则返回 `NULL`。
 */
struct k_component *k_object_add_component(struct k_object *object, struct k_component_type *component_type, void *params);

struct k_object *k_component_get_object(struct k_component *component);

void k_object_del_component(struct k_component *component);

void k_object_del_all_components(struct k_object *object);

// TODO struct k_component *k_object_get_component(struct k_object *object, struct k_component_type *component_type);

// TODO int k_get_objects_with_component(size_t component_type_id, struct k_object ***get_objects, size_t *n);

/* endregion */

/* region [component_get] */

void *k_component_get_data(struct k_component *component);

/* endregion */

/* region [component_add_callback] */

/**
 * \brief 组件回调
 * TODO docs
 */
struct k_component_callback;

struct k_component_callback *k_component_add_step_begin_callback(struct k_component *component, void (*fn_callback)(struct k_component *component));

struct k_component_callback *k_component_add_alarm_callback(struct k_component *component, void (*fn_callback)(struct k_component *component, int timeout_diff), int delay_ms);

struct k_component_callback *k_component_add_step_callback(struct k_component *component, void (*fn_callback)(struct k_component *component));

struct k_component_callback *k_component_add_step_end_callback(struct k_component *component, void (*fn_callback)(struct k_component *component));

struct k_component_callback *k_component_add_draw_callback(struct k_component *component, void (*fn_callback)(struct k_component *component), int z_group, int z_layer);

void k_component_del_callback(struct k_component_callback *callback);

void k_component_del_all_callbacks(struct k_component *component);

/* endregion */

/* region [room_add_component_manager] */

/**
 * \brief 给当前房间添加组件管理器
 *
 * TODO docs
 */
int k_room_add_component_manager(struct k_component_type *component_type, void *params);

/**
 * \brief 获取当前房间的组件管理器
 *
 * 函数返回当前房间所挂载的组件管理器，若没有挂载，则返回 `NULL`。
 */
struct k_component_manager *k_room_get_component_manager(struct k_component_type *component_type);

/* endregion */

/* region [component_manager_get] */

void *k_component_manager_get_data(struct k_component_manager *manager);

/**
 * \brief 获取管理该组件实例的管理器
 *
 * 函数返回与该组件实例关联的管理器。
 *
 * 若创建该组件时，房间没有挂载该组件类型的管理器，
 * 或是该组件类型没有定义管理器，则函数返回 `NULL`。
 */
struct k_component_manager *k_component_get_manager(struct k_component *component);

void *k_component_get_manager_data(struct k_component *component);

/* endregion */

#endif
