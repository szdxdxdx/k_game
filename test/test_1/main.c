#include <stdio.h>

#include "k_game.h"

struct k_image *img;

static int create_room(struct k_room *room, void *unused) {

    struct k_image_config config = {
        .image_name = "image_name",
        .filepath = "assets/tmp.png",
    };

    img = k_load_image(&config);

    return 0;
}

static void room_step(void) {

    k_draw_image(img, NULL, 10, 10);
}

static int init_game(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.steps_per_second = 60;
    config.room_name = "tmp room";
    config.fn_create = create_room;
    k_create_room(&config, NULL);

    struct k_room *tmp_room = k_get_room_by_name("tmp room");

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
