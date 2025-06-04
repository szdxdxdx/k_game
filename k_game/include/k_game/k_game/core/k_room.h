#ifndef K_ROOM_H
#define K_ROOM_H

#include <stddef.h>

#include "./k_game_fwd.h"

/* region [room_create] */

/** \brief 创建房间所需的配置参数 */
struct k_room_config {

    /** \brief 房间的宽高 */
    float room_w;
    float room_h;

    /** \brief 房间的运行速度，单位：步/秒（帧/秒） */
    int room_speed;

    /**
     * \brief 房间关联数据的结构体大小
     *
     * 在此指定房间的自定义关联数据的结构体大小，单位：字节。
     * 创建房间时，k_game 会为关联数据分配内存，
     * 通过 `k_room_get_data()` 可以访问该内存。
     *
     * 关联数据的内存由 k_game 管理，其生命周期与房间相同。
     * 销毁房间时 k_game 会释放该内存。
     *
     * 若不需要关联数据，可将该值设为 0，
     * 之后 `k_room_get_data()` 将返回 `NULL`。
     */
    size_t data_size;

    /**
     * \brief 创建房间时执行的回调
     *
     * 在此回调中，你可以对房间执行你所需的初始化工作：
     * - 初始化该房间自定义数据段的内存
     * - 加载特定资源
     * - 为房间添加事件回调
     * - 在房间中创建对象
     * - ...
     *
     * 函数返回 0 表示成功，非 0 表示失败。
     * 若失败，k_game 将销毁该房间。
     *
     * 入参 `param` 由 `k_room_create()` 传入。
     *
     * 此回调是可选的，设为 `NULL` 则不执行回调。
     */
    int (*on_create)(void *param);

    /**
     * \brief 销毁房间时执行的回调
     *
     * 若房间创建失败，k_game 将直接销毁房间，不调用 `on_destroy()`。
     * 请确保 `on_create()` 能在失败时自行回滚。
     *
     * 此回调是可选的，设为 `NULL` 则不执行回调。
     */
    void (*on_destroy)(void);

    /**
     * \brief 进入房间时执行的回调
     *
     * 此回调是可选的，设为 `NULL` 则不执行回调。
     *
     * TODO enter_event
     * 将来增加 API `struct k_callback * k_room_add_enter_event(void *data, void (*fn)(void *data))`
     * 进入房间时，先执行 `on_enter()` 回调，再执行事件回调
     */
    void (*on_enter)(void);

    /**
     * \brief 离开房间时执行的回调
     *
     * 此回调是可选的，设为 `NULL` 则不执行回调。
     *
     * TODO leave_event
     * 将来增加 API `struct k_callback * k_room_add_leave_event(void *data, void (*fn)(void *data))`
     * 进入房间时，先执行事件回调，再执行 `on_leave()` 回调
     */
    void (*on_leave)(void);
};

/** \brief 创建房间所需的配置参数的默认值 */
#define K_ROOM_CONFIG_INIT \
{ \
    .room_w     = 640,  \
    .room_h     = 480,  \
    .room_speed = 60,   \
    .data_size  = 0,    \
    .on_create  = NULL, \
    .on_destroy = NULL, \
    .on_enter   = NULL, \
    .on_leave   = NULL, \
}

/**
 * \brief 创建房间
 *
 * 函数将入参 `param` 转交给房间的初始化回调 `on_create()`。
 *
 * 若创建成功，函数返回房间指针，否则返回 `NULL`。
 */
struct k_room *k_room_create(const struct k_room_config *config, void *param);

/* endregion */

/* region [room_nav] */

/**
 * \brief 切换房间
 *
 * k_game 框架使用类似页面栈来管理房间的切换。初始时房间栈为空。
 * 用户在游戏的初始化回调中创建房间，之后必须从中指定一个初始房间，进入该房间后，游戏才正式开始。
 *
 * 同一时间只能有一个房间正在运行。处于栈顶的房间即为当前正在运行的房间。
 *
 * @{
 */

/** \brief 直接用新房间替换当前运行的房间 */
void k_room_nav_goto(struct k_room *room);

/** \brief 将一个新房间压入房间栈顶，而原有的房间依然保留在栈中 */
void k_room_nav_push(struct k_room *room);

/** \brief 弹出栈顶房间，回到上一个房间继续运行，若房间栈已空则结束游戏 */
void k_room_nav_pop(void);

/** @} */

/* endregion */

/* region [room_get] */

/** \brief 获取当前房间的关联数据 */
void *k_room_get_data(void);

/** \brief 获取指定房间的关联数据 */
void *k_room_get_data_of(struct k_room *room);

/** \brief 获取当前房间的宽 */
float k_room_get_w(void);

/** \brief 获取当前房间的高 */
float k_room_get_h(void);

/* endregion */

#endif
