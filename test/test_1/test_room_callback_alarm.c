#include <stdio.h>

#include "k_game.h"

static struct alarm_callback_data {
    struct k_room_callback *callback;
    int data;
} alarm_callbacks[3] = { { NULL, 1 }, { NULL, 2 }, { NULL, 3 } };

static void alarm_callback(void *data, int timeout_diff) {
    struct alarm_callback_data *callback_data = data;

    printf("%d\n", callback_data->data);

    /* 使用 k_room_del_callback 删除自身 */
    k_room_del_callback(callback_data->callback);

    callback_data->callback = NULL;
}

static void step_callback(void *data) {
    struct k_room *room = k_get_current_room();

    if (k_is_key_pressed('1') && NULL == alarm_callbacks[0].callback) {
        alarm_callbacks[0].callback = k_room_add_alarm_callback(room, alarm_callback, &alarm_callbacks[0], 500);
    }
    if (k_is_key_pressed('2') && NULL == alarm_callbacks[1].callback) {
        alarm_callbacks[1].callback = k_room_add_alarm_callback(room, alarm_callback, &alarm_callbacks[1], 1000);
    }
    if (k_is_key_pressed('3') && NULL == alarm_callbacks[2].callback) {
        alarm_callbacks[2].callback = k_room_add_alarm_callback(room, alarm_callback, &alarm_callbacks[2], 1500);
    }

    if (k_is_key_held('D')) {
        if (k_is_key_pressed('1')) {
            k_room_del_callback(alarm_callbacks[0].callback);
            alarm_callbacks[0].callback = NULL;
        }
        if (k_is_key_pressed('2')) {
            k_room_del_callback(alarm_callbacks[1].callback);
            alarm_callbacks[1].callback = NULL;
        }
        if (k_is_key_pressed('3')) {
            k_room_del_callback(alarm_callbacks[2].callback);
            alarm_callbacks[2].callback = NULL;
        }
    }
}

/* endregion */

static int create_room(struct k_room *room, void *unused) {
    k_room_add_step_callback(room, step_callback, NULL);
    return 0;
}

static int init_game(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.room_speed = 30;
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
