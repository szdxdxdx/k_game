
#if 1

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "k_game.h"

static struct k_room_callback *room_step;
static struct k_room_callback *room_alarm;
static struct k_object_callback *object_step;
static struct k_object_callback *object_alarm;

static void fn_object_alarm(struct k_object *object, int timeout_diff) {

    printf("object_alarm\n");
    k_object_del_all_callbacks(object);
}

static void fn_object_step(struct k_object *object) {

    if (k_key_pressed('O')) {
        k_object_add_alarm_callback(object, fn_object_alarm, 1000);
    }
}

static void fn_room_alarm(void *data, int timeout_diff) {

    printf("room_alarm\n");
}

static void fn_room_step(void *data) {
    struct k_room *room = k_get_current_room();

    if (k_key_pressed('R')) {
        k_room_add_alarm_callback(room, fn_room_alarm, NULL, 5000);
    }
}

static int init_room(struct k_room *room, void *params) {
    k_room_add_step_callback(room, fn_room_step, "room_step");

    struct k_object *object = k_object_create(0);
    k_object_add_step_callback(object, fn_object_step);

    return 0;
}

static int init_game(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.fn_init = init_room;
    struct k_room *room = k_room_create(&config, NULL);

    k_goto_room(room);
    return 0;
}

int main(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    struct k_game_config config = K_GAME_CONFIG_INIT;
    config.fn_init = init_game;
    k_game_run(&config);

    return 0;
}

#endif
