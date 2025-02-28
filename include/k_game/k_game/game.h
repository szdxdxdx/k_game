#ifndef K_GAME_GAME_H
#define K_GAME_GAME_H

struct k_game_config {

    const char *window_title;

    int window_w, window_h;

    int (*fn_init)(void);

    void (*fn_cleanup)(void);
};

#define K_GAME_CONFIG_INIT \
{ \
    .window_title = "k_game", \
    .window_h     = 480,      \
    .window_w     = 640,      \
    .fn_init      = NULL,     \
    .fn_cleanup   = NULL,     \
}

int k_run_game(const struct k_game_config *config);

#endif
