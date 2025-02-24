#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "k_game.h"

/* region [my_object] */

struct my_object {
    int count[6];
};

#define object_alarm_X(x) \
static void object_alarm_##x (struct k_object *object, int timeout_diff) { \
    struct my_object *my_object = k_object_get_data(object); \
    my_object->count[ x ]++; \
    k_log_info("count[" #x "]: %d", my_object->count[x]); \
}

object_alarm_X(0)
object_alarm_X(1)
object_alarm_X(2)
object_alarm_X(3)
object_alarm_X(4)
object_alarm_X(5)

static void object_step(struct k_object *object) {
    struct my_object *my_object = k_object_get_data(object);

    if (k_is_key_pressed('0')) k_object_add_alarm_callback(object, object_alarm_0, 0000);
    if (k_is_key_pressed('1')) k_object_add_alarm_callback(object, object_alarm_1, 1000);
    if (k_is_key_pressed('2')) k_object_add_alarm_callback(object, object_alarm_2, 2000);
    if (k_is_key_pressed('3')) k_object_add_alarm_callback(object, object_alarm_3, 3000);
    if (k_is_key_pressed('4')) k_object_add_alarm_callback(object, object_alarm_4, 4000);
    if (k_is_key_pressed('5')) k_object_add_alarm_callback(object, object_alarm_5, 5000);
}

static void create_my_object(void) {
    struct k_object *object = k_create_object(sizeof(struct my_object));

    struct my_object *my_object = k_object_get_data(object);
    my_object->count[0] = 0;
    my_object->count[1] = 0;
    my_object->count[2] = 0;
    my_object->count[3] = 0;
    my_object->count[4] = 0;
    my_object->count[5] = 0;

    k_object_add_step_callback(object, object_step);
}

/* endregion */

/* region [room] */

static int create_room(struct k_room *room, void *params) {
    create_my_object();
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
