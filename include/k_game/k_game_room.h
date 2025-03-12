#ifndef K_GAME_ROOM_H
#define K_GAME_ROOM_H

#include <stddef.h>

/**
 * \brief 房间
 *
 * TODO docs
 */
struct k_room;

/** \brief 创建房间所需的配置参数 */
struct k_room_config {

    /** \brief 房间的宽高 */
    int room_w;
    int room_h;

    /** \brief 房间的运行速度，单位：步/秒（帧/秒） */
    int room_speed;

    /**
     * \brief 房间关联数据的结构体大小
     *
     * 在此指定为房间附加的自定义数据的结构体大小，单位：字节。
     *
     * 创建房间时，k_game 会为房间的关联数据分配内存，
     * 之后你可以通过 `k_room_get_data()` 读写该内存。
     *
     * 关联数据的内存由 k_game 管理，其生命周期与房间相同。
     * 销毁房间时 k_game 会释放该内存。
     *
     * 关联数据是可选的，若不需要可指定该值为 0。
     */
    size_t data_size;

    /**
     * \brief 创建房间后执行的回调
     *
     * 在此回调中，你可以对房间执行你所需的初始化工作：
     * - 初始化该房间自定义数据段的内存
     * - 加载特定资源
     * - 为房间添加事件回调
     * - 在房间中创建对象
     * - ...
     *
     * 你需要通过返回值告诉 k_game 你的初始化是否成功，
     * 若成功，回调函数应返回 0，否则应返回非 0。
     * 房间初始化失败，也意味着房间创建失败。
     *
     * 此回调的入参 `params` 由 `k_room_create()` 传入。
     *
     * 此回调是可选的，指定值为 `NULL` 则不执行回调。
     */
    int (*fn_init)(void *params);

    /**
     * \brief 销毁房间前执行的回调
     *
     * 若你在房间运行过程中申请有不是由 k_game 所管辖的资源，
     * 你可以在此回调中释放这些资源。
     *
     * 注意：房间 `fn_init()` 初始化失败后不会执行本回调，
     * 你需保证初始化回调能在失败时自行回滚。
     *
     * 此回调是可选的，指定值为 `NULL` 则不执行回调。
     */
    void (*fn_cleanup)(void);

    void (*fn_enter)(void);

    void (*fn_leave)(void);
};

/** \brief 创建房间所需的配置参数的默认值 */
#define K_ROOM_CONFIG_INIT \
{ \
    .room_w     = 600,  \
    .room_h     = 480,  \
    .room_speed = 60,   \
    .data_size  = 0,    \
    .fn_init    = NULL, \
    .fn_cleanup = NULL, \
    .fn_enter   = NULL, \
    .fn_leave   = NULL, \
}

/**
 * \brief 创建房间
 *
 * 函数将入参 `params` 转交给房间的初始化回调 `fn_init()`。
 *
 * 若创建成功，函数返回房间指针，否则返回 `NULL`。
 */
struct k_room *k_room_create(const struct k_room_config *config, void *params);

/**
 * \brief 设置房间的名字
 *
 * 房间名字是可选的，默认情况下房间没有名字。
 *
 * 你可以为房间设置唯一名字，k_game 将基于该名字为房间建立索引，
 * 之后可使用 `k_room_find()` 根据名字查找房间。
 *
 * 若名字设为空字符串 "" 或 `NULL`，则清除名字，并删除索引。
 *
 * 注意：k_game 仅保存该指针，而不会复制 `room_name` 字符串。
 * 请确保该字符串的内存段在整个使用期间有效且不被修改。
 *
 * 若成功，函数返回 0，否则返回非 0。
 */
int k_room_set_name(struct k_room *room, const char *room_name);

/**
 * \brief 通过房间名称查找房间
 *
 * 若找到，函数返回房间指针，否则返回 `NULL`。
 */
struct k_room *k_room_find(const char *room_name);

int k_goto_room(struct k_room *room);

struct k_room *k_get_current_room(void);

int k_room_get_width(struct k_room *room);

int k_room_get_height(struct k_room *room);

/**
 * \brief 获取房间的关联数据
 *
 * 若创建房间时，若指定了房间关联数据的结构体大小不为 0，
 * 则函数返回该指向数据内存段的指针，否则返回 `NULL`，
 */
void *k_room_get_data(struct k_room *room);

/**
 * \brief 房间回调
 *
 * TODO docs
 */
struct k_room_callback;

struct k_room_callback *k_room_add_step_begin_callback(struct k_room *room, void *data, void (*fn_callback)(void *data));

struct k_room_callback *k_room_add_alarm_callback(struct k_room *room, void *data, void (*fn_callback)(void *data, int timeout_diff), int delay_ms);

struct k_room_callback *k_room_add_step_callback(struct k_room *room, void *data, void (*fn_callback)(void *data));

struct k_room_callback *k_room_add_draw_callback(struct k_room *room, void *data, void (*fn_callback)(void *data), int z_index);

struct k_room_callback *k_room_add_step_end_callback(struct k_room *room, void *data, void (*fn_callback)(void *data));

/*
 * 若 `callback` 为 `NULL`，则函数不做任何事情。
 *
 * TODO docs
 * 删除后最好将指针置为 `NULL`，防止悬空指针
 */
void k_room_del_callback(struct k_room_callback *callback);

void k_room_clean_canvas(void *unused);

#endif
