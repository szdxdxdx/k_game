#include <stdio.h>

#include "k/game.h"

static int setup(void) {

    printf("hello\n");

    return 0;
}

#if 1

int main(int argc, char **argv) {

    struct k_game_config config;
    config.fn_game_setup = setup;

    k_run_game(&config);

    return 0;
}

#endif
