#ifndef K_GAME_RUN_H
#define K_GAME_RUN_H

struct k_game_config {

    const char *window_title;

    int window_w;
    int window_h;

    int canvas_w;
    int canvas_h;

    int (*fn_init)(void);

    void (*fn_fini)(void);
};

#define K_GAME_CONFIG_INIT \
{ \
    .window_title = "",   \
    .window_h     = 480,  \
    .window_w     = 640,  \
    .canvas_w     = 0,    \
    .canvas_h     = 0,    \
    .fn_init      = NULL, \
    .fn_fini      = NULL, \
}

int k_game_run(const struct k_game_config *config);

#endif
