#ifndef K_GAME_ROOM_H
#define K_GAME_ROOM_H

#include <stddef.h>

struct k_room;

struct k_room_config {

    /**
     * \brief 房间的名字
     *
     * 名字是可选的，传递空字符串 "" 或 NULL 表示不使用名字。
     *
     * 若指定名字，需保证其唯一性。程序会基于该名字为房间创建索引，
     * 之后你可以通过 `k_get_room_by_name()` 查找房间。
     *
     * 在创建房间时，程序不会分配内存来复制名字。因此，传递的字符串必须始终有效，
     * 建议使用字符串字面量作为房间名字，它们的内存会在程序生命周期内保持有效。
     */
    const char *room_name;

    /**
     * \brief 房间的宽高
     */
    int room_w, room_h;

    /**
     * \brief 房间的运行速度
     */
    int steps_per_second;

    size_t data_size;

    int (*fn_create)(struct k_room *room, void *params);

    void (*fn_destroy)(struct k_room *room);
};

extern const struct k_room_config K_ROOM_CONFIG_INIT;

struct k_room *k_create_room(const struct k_room_config *config, void *params);

/* ------------------------------------------------------------------------ */

struct k_room *k_get_current_room(void);

/**
 * \brief 通过房间名字查找对应的房间
 *
 * 当创建房间时，如果给定了房间名字，程序会基于该名字为房间创建索引。
 * 本函数能根据房间名字查找对应的房间。
 *
 * \param room_name 房间的名字
 * \return 若找到，函数返回房间指针，否则返回 NULL。
 *         若传入的名字为空字符串 "" 或 NULL，则函数返回 NULL。
 */
struct k_room *k_get_room_by_name(const char *room_name);

const char *k_room_get_name(struct k_room *room);

int k_room_get_width(struct k_room *room);

int k_room_get_height(struct k_room *room);

float k_room_get_speed(struct k_room *room);

void *k_room_get_data(struct k_room *room);

void *k_get_current_room_data(void);

void k_room_set_speed(struct k_room *room, int steps_per_second);

/* ------------------------------------------------------------------------ */

int k_goto_room(struct k_room *room);

struct k_room *k_room_stack_get_top(void);

/* ------------------------------------------------------------------------ */

struct k_room_callback;

struct k_room_callback *k_room_add_enter_callback(struct k_room *room, void (*fn_callback)(void *data), void *data);

struct k_room_callback *k_room_add_leave_callback(struct k_room *room, void (*fn_callback)(void *data), void *data);

struct k_room_callback *k_room_add_step_begin_callback(struct k_room *room, void (*fn_callback)(void *data), void *data);

struct k_room_callback *k_room_add_alarm_callback(struct k_room *room, void (*fn_callback)(void *data, int timeout_diff), void *data, int delay);

struct k_room_callback *k_room_add_step_callback(struct k_room *room, void (*fn_callback)(void *data), void *data);

struct k_room_callback *k_room_add_draw_callback(struct k_room *room, void (*fn_callback)(void *data), void *data, int depth);

struct k_room_callback *k_room_add_step_end_callback(struct k_room *room, void (*fn_callback)(void *data), void *data);

void k_room_del_callback(struct k_room_callback *callback);

#endif
