#ifndef K_GAME_ROOM_H
#define K_GAME_ROOM_H

#include <stddef.h>

struct k_room;

struct k_room_config {

    /**
     * \brief 房间的名字
     *
     * 房间名字是可选的。若不需要名字，你可以传递空字符串 "" 或 NULL。
     *
     * 若指定了名字，你需要保证该名字是唯一的。房间名字被用于建立房间索引。
     * 若你指定了房间名字，之后可以使用 `k_get_room_by_name()` 根据名字找到房间。
     *
     * 在创建房间时，程序不会分配内存来复制房间名字。因此，传递的字符串必须始终有效，
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
