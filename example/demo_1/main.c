#include <stdio.h>

#include "k/game.h"

struct k_image *img;

static int entry_room(const struct k_room *room) {
    printf("\nentry my room\n\n");

    img = k_load_image("assets/tmp.png");

    return 0;
}

static void room_step(const struct k_room *room) {

    k_draw_image(img, 10, 10);
}

static int init_game(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.room_name = "tmp room";
    config.fn_enter  = entry_room;
    config.fn_step   = room_step;
    struct k_room *tmp_room = k_create_room(&config);

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
