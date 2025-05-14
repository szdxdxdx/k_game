#include <stdio.h>
#include <stdlib.h>

#include "k_game.h"

static int on_game_start(void) {

    return 0;
}

int main(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    struct k_game_config config = K_GAME_CONFIG_INIT;
    config.window_w     = 640;
    config.window_h     = 480;
    config.window_title = "ynx! llk! fight!";
    config.on_start     = on_game_start;

    return k_game_run(&config);;
}
