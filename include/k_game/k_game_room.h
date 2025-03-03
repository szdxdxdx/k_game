#ifndef K_GAME_ROOM_H
#define K_GAME_ROOM_H

#include <stddef.h>

/**
 * \brief 房间
 *
 * TODO docs
 */
struct k_room;

/* region [create_room] */

/** \brief 创建房间所需的配置参数 */
struct k_room_config {

    /**
     * \brief 房间的名字
     *
     * 名字是可选的，指定值为空字符串 "" 或 `NULL` 表示不使用名字。
     *
     * 若指定名字，需保证其唯一性。k_game 会基于该名字为房间创建索引，
     * 之后你可以通过 `k_get_room_by_name()` 查找房间。
     *
     * 在创建房间时，k_game 不会分配内存来复制名字。
     * 传递的字符串的内存段必须始终有效，建议使用字符串字面量。
     */
    const char *room_name;

    /** \brief 房间的宽高 */
    int room_w, room_h;

    /** \brief 房间的运行速度，单位：步/秒（帧/秒） */
    int room_speed;

    /**
     * \brief 自定义数据结构体的大小，单位：字节
     *
     * 在此指定为房间附加的自定义数据结的构体大小。
     *
     * 创建房间时，k_game 会为自定义的数据段分配内存，
     * 之后你可以通过 `k_room_get_data()` 访问该数据。
     *
     * 附加数据是可选的，若不需要可指定该值为 0。
     */
    size_t data_size;

    /**
     * \brief 创建房间时的初始化回调
     *
     * 在此回调中，你可以对房间执行你所需的初始化工作：
     * - 初始化该房间自定义数据段的内存
     * - 加载特定资源
     * - 为房间添加事件回调
     * - 在房间中创建对象
     * - ...
     *
     * 房间初始化失败，也意味着房间创建失败。
     * 你将通过函数的返回值告诉 k_game 你的初始化是否成功，
     * 若成功，函数应返回 0，否则应返回非 0。
     *
     * 此回调是可选的，指定值为 `NULL` 则不执行回调。
     */
    int (*fn_init)(struct k_room *room, void *params);

    /**
     * \brief 销毁房间时的清理回调
     *
     * 若你在房间运行过程中申请有不是由 k_game 所管辖的资源，
     * 你可以在此回调中释放这些资源。
     *
     * 此回调是可选的，指定值为 `NULL` 则不执行回调。
     *
     * 注意，若房间的初始化回调执行失败，k_game 将不执行本回调。
     * 你需保证初始化回调能在失败时自行回滚。
     */
    void (*fn_cleanup)(struct k_room *room);
};

/** \brief 创建房间所需的配置参数的默认值 */
#define K_ROOM_CONFIG_INIT \
{ \
    .room_name  = NULL, \
    .room_w     = 600,  \
    .room_h     = 480,  \
    .room_speed = 60,   \
    .data_size  = 0,    \
    .fn_init    = NULL, \
    .fn_cleanup = NULL  \
}

/**
 * \brief 创建房间
 *
 * 若 `config` 中指定了房间的初始化回调，
 * 则函数将 `params` 转交给该回调。
 *
 * 若创建成功，函数返回房间指针，否则返回 `NULL`。
 */
struct k_room *k_create_room(const struct k_room_config *config, void *params);

/* endregion */

/* region [room_get] */

/**
 * \brief 通过房间名字查找对应的房间
 *
 * 当创建房间时，若指定了房间的名字，k_game 会基于该名字为房间创建索引。
 * 本函数能根据名字查找到对应的房间。
 * 若找到，函数返回房间指针，否则返回 `NULL`。
 */
struct k_room *k_get_room_by_name(const char *room_name);

struct k_room *k_get_current_room(void);

/**
 * \brief 获取房间的名字
 *
 * 若创建该房间时指定了名字，则函数返回其名字，否则返回空字符串 ""。
 * 你不应该释放该字符串。
 */
const char *k_room_get_name(struct k_room *room);

int k_room_get_width(struct k_room *room);

int k_room_get_height(struct k_room *room);

void *k_room_get_data(struct k_room *room);

void *k_get_current_room_data(void);

/* endregion */

/* region [room_goto] */

int k_goto_room(struct k_room *room);

/* endregion */

/**
 * \brief 房间回调
 *
 * TODO docs
 */
struct k_room_callback;

/* region [room_callback] */

struct k_room_callback *k_room_add_enter_callback(struct k_room *room, void (*fn_callback)(void *data), void *data);

struct k_room_callback *k_room_add_leave_callback(struct k_room *room, void (*fn_callback)(void *data), void *data);

struct k_room_callback *k_room_add_step_begin_callback(struct k_room *room, void (*fn_callback)(void *data), void *data);

struct k_room_callback *k_room_add_alarm_callback(struct k_room *room, void (*fn_callback)(void *data, int timeout_diff), void *data, int delay_ms);

struct k_room_callback *k_room_add_step_callback(struct k_room *room, void (*fn_callback)(void *data), void *data);

struct k_room_callback *k_room_add_draw_callback(struct k_room *room, void (*fn_callback)(void *data), void *data, int z_index);

struct k_room_callback *k_room_add_step_end_callback(struct k_room *room, void (*fn_callback)(void *data), void *data);

/*
 * 若 `callback` 为 `NULL`，则函数不做任何事情。
 *
 * TODO docs
 * 删除后最好将指针置为 `NULL`，防止悬空指针
 */
void k_room_del_callback(struct k_room_callback *callback);

/* endregion */

#endif
