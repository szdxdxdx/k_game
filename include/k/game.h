#ifndef K_GAME_H
#define K_GAME_H

struct k_game_config;

int k_run_game(const struct k_game_config *config);

struct k_game_config {

    int (*fn_game_setup)(void);
};

#endif
