#include <stdio.h>

#include "k_game.h"

struct k_image *img;

struct k_room_callback *callback_print_num;

static void enter(void *unused) {
    printf("enter\n");
}

static void timer_1000ms(void *unused, int timeout_diff) {
    printf("timer 1000ms, diff %d\n", timeout_diff);

    struct k_room *room = k_get_current_room();
    k_room_add_alarm_callback(room, timer_1000ms, NULL, 1000);
}

static void timer_2000ms(void *unused, int timeout_diff) {
    printf("timer   2000ms, diff %d\n", timeout_diff);

    struct k_room *room = k_get_current_room();
    k_room_add_alarm_callback(room, timer_2000ms, NULL, 2000);
}

static void step(void *unused) {
    printf("step\n");
}

static void print_num(void *data) {
    int *num = data;

    printf("%d\n", ++(*num));

    if (*num >= 45) {
        k_room_del_callback(callback_print_num);

        struct k_room *room = k_get_current_room();
        k_room_add_alarm_callback(room, timer_1000ms, NULL, 1000);
        k_room_add_alarm_callback(room, timer_2000ms, NULL, 1500);
    }
}

static void draw_img(void *unused) {
    k_draw_image(img, NULL, 0, 0);
    printf("draw img\n");
}

static void draw(void *data) {
    int *num = data;
    printf("draw %d\n", *num);
}

static int create_room(struct k_room *room, void *unused) {

    k_room_add_enter_callback(room, enter, NULL);

    // k_room_add_step_callback(room, step, NULL);

    // static int g_data;
    // callback_print_num = k_room_add_step_callback(room, print_num, &g_data);

    // k_room_add_draw_callback(room, draw_img, NULL, -1);

    // static int draw_data[4] = { -2, 0, 2, 3 };
    // k_room_add_draw_callback(room, draw, &draw_data[2], draw_data[2]);
    // k_room_add_draw_callback(room, draw, &draw_data[3], draw_data[3]);
    // k_room_add_draw_callback(room, draw, &draw_data[0], draw_data[0]);
    // k_room_add_draw_callback(room, draw, &draw_data[1], draw_data[1]);
    // k_room_add_draw_callback(room, draw, &draw_data[2], draw_data[2]);

    return 0;
}

static int init_game(void) {

    {
        struct k_image_config config = { "", "assets/tmp.png", };
        img = k_load_image(&config);
    }

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.steps_per_second = 60;
    config.room_name = "tmp room";
    config.fn_create = create_room;
    struct k_room *tmp_room = k_create_room(&config, NULL);

    k_goto_room(tmp_room);

    return 0;
}

#if 1

int main(int argc, char **argv) {
    setbuf(stdout, NULL);

    struct k_game_config config = K_GAME_CONFIG_INIT;

    config.fn_init = init_game;

    k_run_game(&config);

    return 0;
}

#endif
