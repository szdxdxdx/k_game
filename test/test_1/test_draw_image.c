#include <stdio.h>

#include "k_game.h"

static struct k_image *img;

static void draw_img(void *unused) {
    k_draw_image(img, NULL, 0, 0);
}

static int create_room(struct k_room *room, void *unused) {

    int depth = -1;
    k_room_add_draw_callback(room, draw_img, NULL, depth);

    return 0;
}

static int init_game(void) {

    img = k_load_image("", "assets/tmp.png");

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
    config.window_w = 400;
    config.window_h = 400;
    config.fn_init = init_game;
    k_run_game(&config);

    return 0;
}

#endif
