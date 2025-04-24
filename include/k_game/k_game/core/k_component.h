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
     * 创建组件实例时，k_game 会为关联数据分配内存，
     * 通过 `k_component_get_data()` 可以访问该内存。
     *
     * 关联数据的内存由 k_game 管理，其生命周期与组件相同。
     * 销毁组件时 k_game 会释放该内存。
     *
     * 若不需要关联数据，可将该值设为 0，
     * 之后 `k_component_get_data()` 将返回 `NULL`。
     */
    size_t data_size;

    /**
     * \brief 创建组件实例后执行的回调
     *
     * 必须指定该回调，用于在组件创建后执行必要的初始化工作。
     * k_game 在创建组件实例，并将其挂载到对象上后，会执行此回调。
     *
     * 入参 `params` 由 `k_object_add_component()` 传入。
     *
     * 在此回调中，你需要执行组件的初始化工作，例如：
     * - 初始化自定义关联数据的内存
     * - 为组件添加事件回调
     * - ...
     *
     * 函数返回 0 表示初始化成功，非 0 表示初始化失败。
     * 若初始化失败，k_game 将卸载并销毁该组件。
     */
    int (*fn_init)(struct k_component *component, void *params);

    /**
     * \brief 销毁组件实例前执行的回调
     *
     * 若组件初始化失败，k_game 将直接销毁组件，不调用 `fn_fini()`。
     * 请确保 `fn_init()` 能在初始化失败时自行回滚。
     *
     * 此回调是可选的，若不需要，可将该值设为 `NULL`。
     */
    void (*fn_fini)(struct k_component *component);

    /* TODO */
    int (*fn_interact)(struct k_component *component, char *get_result, size_t n, const char *command);
};

/** \brief 注册组件类型所需的组件配置的默认值 */
#define K_COMPONENT_ENTITY_CONFIG_INIT \
{ \
    .data_size = 0,    \
    .fn_init   = NULL, \
    .fn_fini   = NULL  \
}

/** \brief 注册组件类型所需的组件管理器配置 */
struct k_component_manager_config {

    /**
     * \brief 关联数据的结构体大小
     *
     * 指定组件管理器的自定义关联数据的结构体大小，单位：字节。
     * 创建组件管理器时，k_game 会为关联数据分配内存，
     * 通过 `k_component_manager_get_data()` 可以访问该内存。
     *
     * 关联数据的内存由 k_game 管理，其生命周期与组件相同。
     * 销毁组件管理器时 k_game 会释放该内存。
     *
     * 若不需要关联数据，可将该值设为 0，
     * 之后 `k_component_manager_get_data()` 将返回 `NULL`。
     */
    size_t data_size;

    /**
     * \brief 创建组件管理器后执行的回调
     *
     * k_game 在创建组件管理器，并将其添加到房间后，会执行此回调。
     *
     * 入参 `params` 由 `k_room_add_component_manager()` 传入。
     *
     * 在此回调中，你需要执行组件管理器的初始化工作，例如：
     * - 初始化自定义关联数据的内存
     * - 为组件管理器添加事件回调
     * - ...
     *
     * 函数返回 0 表示初始化成功，非 0 表示初始化失败。
     * 若初始化失败，k_game 将移除并销毁该组件。
     *
     * 此回调是可选的，若不需要，可将该值设为 `NULL`。
     */
    int (*fn_init)(struct k_component_manager *manager, void *params);

    /**
     * \brief 销毁组件管理器前执行的回调
     *
     * 若组件管理器初始化失败，k_game 将直接销毁管理器，不调用 `fn_fini()`。
     * 请确保 `fn_init()` 能在初始化失败时自行回滚。
     *
     * 此回调是可选的，若不需要，可将该值设为 `NULL`。
     */
    void (*fn_fini)(struct k_component_manager *manager);
};

/** \brief 注册组件类型所需的组件管理器配置的默认值 */
#define K_COMPONENT_MANAGER_CONFIG_INIT \
{ \
    .data_size = 0,    \
    .fn_init   = NULL, \
    .fn_fini   = NULL  \
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
 * 入参 `params` 被转交给组件的初始化回调 `fn_init()`。
 *
 * 若挂载成功，函数返回组件的指针，否则返回 `NULL`。
 */
struct k_component *k_object_add_component(struct k_object *object, struct k_component_type *component_type, void *params);

/**
 * \brief 移除对象上挂载的组件
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
 * 入参 `params` 被转交给组件管理器的初始化回调 `fn_init()`。
 *
 * 若添加成功，函数返回组件管理器的指针，否则返回 `NULL`。
 */
int k_room_add_component_manager(struct k_component_type *component_type, void *params);

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
 * 若注册组件类型时没有定义管理器，则函数返回 `NULL`。
 * 若创建该组件时，房间还没有添加管理器，则函数返回 `NULL`。
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
