#ifndef K_GAME_ROOM_H
#define K_GAME_ROOM_H

#include <stddef.h>

struct k_room;

struct k_room_config {

    const char *room_name;

    int room_w, room_h;

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

/* ------------------------------------------------------------------------ */

struct k_room_enter_callback;

struct k_room_enter_callback *k_room_add_enter_callback(struct k_room *room, void (*fn_callback)(void *data), void *data);

void k_room_del_enter_callback(struct k_room_enter_callback *callback);

/* ------------------------------------------------------------------------ */

struct k_room_leave_callback;

struct k_room_leave_callback *k_room_add_leave_callback(struct k_room *room, void (*fn_callback)(void *data), void *data);

void k_room_del_leave_callback(struct k_room_leave_callback *callback);

/* ------------------------------------------------------------------------ */

struct k_room_step_begin_callback;

struct k_room_step_begin_callback *k_room_add_step_begin_callback(struct k_room *room, void (*fn_callback)(void *data), void *data);

void k_room_del_step_begin_callback(struct k_room_step_begin_callback *callback);

/* ------------------------------------------------------------------------ */

struct k_room_alarm_callback;

struct k_room_alarm_callback *k_room_add_alarm_callback(struct k_room *room, void (*fn_callback)(void *data, float timeout), void *data, float delay);

void k_room_del_alarm_callback(struct k_room_alarm_callback *callback);

/* ------------------------------------------------------------------------ */

struct k_room_step_callback;

struct k_room_step_callback *k_room_add_step_callback(struct k_room *room, void (*fn_callback)(void *data), void *data);

void k_room_del_step_callback(struct k_room_step_callback *callback);

/* ------------------------------------------------------------------------ */

struct k_room_draw_callback;

struct k_room_draw_callback *k_room_add_draw_callback(struct k_room *room, void (*fn_callback)(void *data), void *data, int depth);

void k_room_del_draw_callback(struct k_room_draw_callback *callback);

void k_room_draw_callback_set_depth(struct k_room_draw_callback *callback, int depth);

/* ------------------------------------------------------------------------ */

struct k_room_step_end_callback;

struct k_room_step_end_callback *k_room_add_step_end_callback(struct k_room *room, void (*fn_callback)(void *data), void *data);

void k_room_del_step_end_callback(struct k_room_step_end_callback *callback);

#endif
