#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "k_game.h"

/* region [my_object] */

struct my_object {
    struct k_object_callback *callback;
    int count;
};

static void object_step(struct k_object *object) {
    struct my_object *my_object = k_object_get_data(object);

    my_object->count++;
    printf("count: %d\n", my_object->count);

    if (my_object->count >= 50)
        k_object_del_callback(my_object->callback);
}

/* endregion */

/* region [room] */

static int create_room(struct k_room *room, void *params) {

    struct k_object *object = k_create_object(sizeof(struct my_object));

    struct my_object *my_object = k_object_get_data(object);

    my_object->count = 0;
    my_object->callback = k_object_add_step_callback(object, object_step);

    return 0;
}

/* endregion */

/* region [game] */

static int init(void) {

    struct k_room_config room_config = K_ROOM_CONFIG_INIT;
    room_config.room_name = "room";
    room_config.fn_create = create_room;

    struct k_room *room = k_create_room(&room_config, NULL);

    k_goto_room(room);
    return 0;
}

#if 0

int main(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    struct k_game_config game_config = K_GAME_CONFIG_INIT;
    game_config.window_w = 400;
    game_config.window_h = 360;
    game_config.fn_init = init;

    k_run_game(&game_config);

    return 0;
}

#endif

/* endregion */
