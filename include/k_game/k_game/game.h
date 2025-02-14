#ifndef K_GAME_GAME_H
#define K_GAME_GAME_H

struct k_game_config {

    const char *window_title;

    int window_w, window_h;

    int (*fn_init)(void);

    void (*fn_cleanup)(void);
};

extern const struct k_game_config K_GAME_CONFIG_INIT;

int k_run_game(const struct k_game_config *config);

#endif
