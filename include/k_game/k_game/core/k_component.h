#ifndef K_COMPONENT_H
#define K_COMPONENT_H

#include <stddef.h>

#include "./k_game_fwd.h"

/* region [component_type_register] */

/** \brief 注册组件类型所需的组件配置 */
struct k_component_entity_config {

    /**
     * \brief 关联数据的结构体大小
     *
     * 指定组件的自定义关联数据的结构体大小，单位：字节。
     * 关联数据的内存由 k_game 负责分配和释放，其生命周期与组件一致。
     * 可通过 `k_component_get_data()` 访问该内存。
     *
     * 若不需要关联数据，可将该值设为 0，
     * 之后 `k_component_get_data()` 将返回 `NULL`。
     */
    size_t data_size;

    /**
     * \brief 创建组件实例时执行的回调
     *
     * 必须指定该回调，用于在组件创建后执行必要的初始化工作。
     * k_game 在创建组件实例，并将其挂载到对象上后，会执行此回调。
     *
     * 入参 `param` 由 `k_object_add_component()` 传入。
     *
     * 在此回调中，你需要执行组件的初始化工作，例如：
     * - 初始化自定义关联数据的内存
     * - 为组件添加事件回调
     * - ...
     *
     * 函数返回 0 表示成功，非 0 表示失败。
     * 若失败，k_game 将卸载并销毁该组件。
     */
    int (*on_create)(struct k_component *component, void *param);

    /**
     * \brief 销毁组件实例时执行的回调
     *
     * 若组件创建失败，k_game 将直接销毁组件，不调用 `on_destroy()`。
     * 请确保 `on_create()` 能在失败时自行回滚。
     *
     * 此回调是可选的，若不需要，可将该值设为 `NULL`。
     */
    void (*on_destroy)(struct k_component *component);
};

/** \brief 注册组件类型所需的组件配置的默认值 */
#define K_COMPONENT_ENTITY_CONFIG_INIT \
{ \
    .data_size  = 0,    \
    .on_create  = NULL, \
    .on_destroy = NULL  \
}

/** \brief 注册组件类型所需的组件管理器配置 */
struct k_component_manager_config {

    /**
     * \brief 关联数据的结构体大小
     *
     * 指定组件管理器的自定义关联数据的结构体大小，单位：字节。
     * 关联数据的内存由 k_game 负责分配和释放，其生命周期与组件管理器一致。
     * 可通过 `k_component_manager_get_data()` 访问该内存。
     *
     *
     * 若不需要关联数据，可将该值设为 0，
     * 之后 `k_component_manager_get_data()` 将返回 `NULL`。
     */
    size_t data_size;

    /**
     * \brief 创建组件管理器时执行的回调
     *
     * k_game 在创建组件管理器，并将其添加到房间后，会执行此回调。
     *
     * 入参 `param` 由 `k_room_add_component_manager()` 传入。
     *
     * 在此回调中，你需要执行组件管理器的初始化工作，例如：
     * - 初始化自定义关联数据的内存
     * - 为组件管理器添加事件回调
     * - ...
     *
     * 函数返回 0 表示成功，非 0 表示失败。
     * 若失败，k_game 将移除并销毁该组件。
     *
     * 此回调是可选的，若不需要，可将该值设为 `NULL`。
     */
    int (*on_create)(struct k_component_manager *manager, void *param);

    /**
     * \brief 销毁组件管理器时执行的回调
     *
     * 若组件管理器创建失败，k_game 将直接销毁管理器，不调用 `on_destroy()`。
     * 请确保 `on_create()` 能在失败时自行回滚。
     *
     * 此回调是可选的，若不需要，可将该值设为 `NULL`。
     */
    void (*on_destroy)(struct k_component_manager *manager);
};

/** \brief 注册组件类型所需的组件管理器配置的默认值 */
#define K_COMPONENT_MANAGER_CONFIG_INIT \
{ \
    .data_size  = 0,    \
    .on_create  = NULL, \
    .on_destroy = NULL  \
}

/**
 * \brief 注册组件类型
 *
 * 向 k_game 注册一个新的组件类型，此后可通过该类型创建组件实例。
 *
 * 必须提供组件实例的配置 `entity_config`。
 * 管理器配置 `manager_config` 则是可选的，为 `NULL` 表示该组件类型无需管理器。
 *
 * 若注册成功，函数返回组件类型指针，注册失败则返回 NULL。
 */
struct k_component_type *k_component_type_register(const struct k_component_manager_config *manager_config, const struct k_component_entity_config *entity_config);

/* endregion */

/* region [component_type_find] */

/**
 * \brief 设置组件类型的名字
 *
 * 你可以为已注册的组件类型指定一个名字，名字必须唯一。
 * 之后可使用 `k_component_type_find()` 查找该组件类型。
 *
 * 名字不是必须的，你可以直接保存组件类型的指针。
 *
 * 若名字设为空字符串 "" 或 `NULL`，则清除名字。
 *
 * k_game 不会复制名字，而仅是保存字符串指针。
 * 请确保该字符串的内存始终有效，且内容不被修改。
 * 推荐使用字符串字面量或静态存储的字符串。
 *
 * 若设置成功，函数返回 0，否则返回非 0。
 */
int k_component_type_set_name(struct k_component_type *component_type, const char *type_name);

/**
 * \brief 根据名字查找组件类型
 *
 * 若找到，函数返回组件类型的指针，找不到则返回 `NULL`。
 */
struct k_component_type *k_component_type_find(const char *type_name);

/* endregion */

/* region [object_add_component] */

/**
 * \brief 往对象上挂载组件
 *
 * 函数创建组件实例，并将其挂载到指定对象上。
 * 入参 `param` 被转交给组件的初始化回调 `on_create()`。
 *
 * 若挂载成功，函数返回组件的指针，否则返回 `NULL`。
 */
struct k_component *k_object_add_component(struct k_object *object, struct k_component_type *component_type, void *param);

/**
 * \brief 从对象上移除组件
 *
 * 函数将组件从其所属对象上移除并销毁。
 * 销毁对象时，其挂载的所有组件会被一并移除，无需手动调用本函数。
 *
 * 若 `component` 为 `NULL` 则函数立即返回。
 */
void k_object_del_component(struct k_component *component);

/* endregion */

/* region [room_add_component_manager] */

/**
 * \brief 往当前房间添加组件管理器
 *
 * 函数创建组件管理器实例，并将其添加到当前房间中。
 * 入参 `param` 被转交给组件管理器的初始化回调 `on_create()`。
 *
 * 若添加成功，函数返回组件管理器的指针，否则返回 `NULL`。
 */
int k_room_add_component_manager(struct k_component_type *component_type, void *param);

/* endregion */

/* region [component_get/manager_get] */

/** \brief 获取组件的关联数据 */
void *k_component_get_data(struct k_component *component);

/** \brief 获取该组件所附属的对象 */
struct k_object *k_component_get_object(struct k_component *component);

/**
 * \brief 获取该组件所附属的对象的关联数据
 *
 * 相当于依次调用 `k_component_get_object()` 和 `k_object_get_data()`。
 */
void *k_component_get_object_data(struct k_component *component);

/**
 * \brief 获取管理该组件的管理器
 *
 * 创建组件实例时，组件会与房间中的组件管理器关联（若管理器存在）。
 * 函数返回该组件关联的管理器。
 *
 * 若注册该组件的类型时，没有配置管理器，则函数返回 `NULL`。
 * 若创建该组件时，房间尚未添加管理器，则函数返回 `NULL`。
 */
struct k_component_manager *k_component_get_manager(struct k_component *component);

/**
 * \brief 获取当前房间的组件管理器
 *
 * 函数返回当前房间中的指定组件类型的管理器，若没有则返回 `NULL`。
 */
struct k_component_manager *k_room_get_component_manager(struct k_component_type *component_type);

/** \brief 获取组件管理器的关联数据 */
void *k_component_manager_get_data(struct k_component_manager *manager);

/**
 * \brief 获取该组件的管理器的关联数据
 *
 * 相当于依次调用 `k_component_get_manager()` 和 `k_component_manager_get_data()`。
 */
void *k_component_get_manager_data(struct k_component *component);

/**
 * \brief 获取当前房间的组件管理器的关联数据
 *
 * 相当于依次调用 `k_room_get_component_manager()` 和 `k_component_manager_get_data()`。
 */
void *k_room_get_component_manager_data(struct k_component_type *component_type);

/* endregion */

#endif
