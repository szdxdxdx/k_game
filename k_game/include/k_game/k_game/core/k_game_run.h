#ifndef K_GAME_RUN_H
#define K_GAME_RUN_H

#include <stddef.h>

struct k_game_config {

    const char *window_title;

    int window_w;
    int window_h;

    int (*on_start)(void);

    void (*on_end)(void);
};

#define K_GAME_CONFIG_INIT \
{ \
    .window_title = "", \
    .window_h = 480,  \
    .window_w = 640,  \
    .on_start = NULL, \
    .on_end   = NULL, \
}

int k_game_run(const struct k_game_config *config);

#endif
