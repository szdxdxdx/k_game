#include <stdio.h>

#include "k_game.h"

/* region [enter] */

static void enter_callback_1(void *data) {
    printf("enter callback 1\n");
}

static void enter_callback_2(void *data) {
    printf("enter callback 2\n");
}

/* endregion */

/* region [leave] */

static void leave_callback_1(void *data) {
    printf("leave callback 1\n");
}

static void leave_callback_2(void *data) {
    printf("leave callback 2\n");
}

/* endregion */

/* region [step_begin] */

static void step_begin_callback_1(void *data) {
    printf("step begin callback 1\n");
}

static void step_begin_callback_2(void *data) {
    printf("step begin callback 2\n");
}

static void step_begin_callback_3(void *data) {
    printf("step begin callback 3\n");
}

/* endregion */

/* region [alarm] */

static void alarm_callback_100ms(void *data, int timeout_diff) {
    printf("alarm callback 100ms %d\n", timeout_diff);

    struct k_room *room = k_get_current_room();
    k_room_add_alarm_callback(room, alarm_callback_100ms, NULL, 500 - timeout_diff);
}

static void alarm_callback_500ms(void *data, int timeout_diff) {
    printf("alarm callback 500ms %d \n", timeout_diff);

    struct k_room *room = k_get_current_room();
    k_room_add_alarm_callback(room, alarm_callback_500ms, NULL, 500 - timeout_diff);
}

static void alarm_callback_1000ms(void *data, int timeout_diff) {
    printf("alarm callback 1000ms %d\n", timeout_diff);

    struct k_room *room = k_get_current_room();
    k_room_add_alarm_callback(room, alarm_callback_1000ms, NULL, 1000 - timeout_diff);
}

/* endregion */

/* region [step] */

static void step_callback_1(void *data) {
    printf("step callback 1\n");
}

static void step_callback_2(void *data) {
    printf("step callback 2\n");
}

static void step_callback_3(void *data) {
    printf("step callback 3\n");
}

/* endregion */

/* region [draw] */

static void draw_callback_depth_1(void *data) {
    printf("draw callback depth 1\n");
}

static void draw_callback_depth_2(void *data) {
    printf("draw callback depth 2\n");
}

static void draw_callback_depth_3(void *data) {
    printf("draw callback depth 3\n");
}

static void draw_callback_depth_4(void *data) {
    printf("draw callback depth 4\n");
}

/* endregion */

/* region [step_end] */

static void step_end_callback_1(void *data) {
    printf("step end callback 1\n");
}

static void step_end_callback_2(void *data) {
    printf("step end callback 2\n");
}

static void step_end_callback_3(void *data) {
    printf("step end callback 3\n\n\n");
}

/* endregion */

static int create_room(struct k_room *room, void *unused) {

    k_room_add_enter_callback(room, enter_callback_1, NULL);
    k_room_add_enter_callback(room, enter_callback_2, NULL);

    k_room_add_leave_callback(room, leave_callback_1, NULL);
    k_room_add_leave_callback(room, leave_callback_2, NULL);

    k_room_add_step_begin_callback(room, step_begin_callback_1, NULL);
    k_room_add_step_begin_callback(room, step_begin_callback_2, NULL);
    k_room_add_step_begin_callback(room, step_begin_callback_3, NULL);

    k_room_add_alarm_callback(room, alarm_callback_100ms, NULL, 100);
    k_room_add_alarm_callback(room, alarm_callback_500ms, NULL, 500);
    k_room_add_alarm_callback(room, alarm_callback_1000ms, NULL, 1000);

    k_room_add_step_callback(room, step_callback_1, NULL);
    k_room_add_step_callback(room, step_callback_2, NULL);
    k_room_add_step_callback(room, step_callback_3, NULL);

    k_room_add_step_end_callback(room, step_end_callback_1, NULL);
    k_room_add_step_end_callback(room, step_end_callback_2, NULL);
    k_room_add_step_end_callback(room, step_end_callback_3, NULL);

    k_room_add_draw_callback(room, draw_callback_depth_3, NULL, 3);
    k_room_add_draw_callback(room, draw_callback_depth_1, NULL, 1);
    k_room_add_draw_callback(room, draw_callback_depth_4, NULL, 4);
    k_room_add_draw_callback(room, draw_callback_depth_2, NULL, 2);

    return 0;
}

static int init_game(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.room_speed = 1;
    config.room_name = "tmp room";
    config.fn_init = create_room;
    struct k_room *tmp_room = k_create_room(&config, NULL);

    k_goto_room(tmp_room);

    return 0;
}

#if 0

int main(int argc, char **argv) {
    setbuf(stdout, NULL);

    struct k_game_config config = K_GAME_CONFIG_INIT;

    config.fn_init = init_game;

    k_run_game(&config);

    return 0;
}

#endif
