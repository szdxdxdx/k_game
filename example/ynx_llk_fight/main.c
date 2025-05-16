#include <stdio.h>
#include <stdlib.h>

#include "game/yx_game_init.h"

int main(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    return yx_game_run();
}
