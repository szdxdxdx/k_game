#include <stdio.h>

#include "k_game.h"

struct k_image *img;

struct k_room_callback *callback_print_num;

static void print_num(void *data) {
    int *num = data;

    printf("%d\n", ++(*num));

    if (*num > 200)
        k_room_del_callback(callback_print_num);
}

static int create_room(struct k_room *room, void *unused) {

    static int g_data;
    callback_print_num = k_room_add_step_callback(room, print_num, &g_data);

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
