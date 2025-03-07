
#if 1

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "k_game.h"

static void object_step(struct k_object *object) {
    printf("object_step\n");
}

static void room_step(void *data) {

    const char *s = data;

    printf("%s\n", s);
}

static int init_room(struct k_room *room, void *params) {
    k_room_add_step_callback(room, room_step, "room_step");

    struct k_object *object = k_object_create(0);
    k_object_add_step_callback(object, object_step);

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
