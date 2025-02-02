#ifndef K_GAME_INIT_H
#define K_GAME_INIT_H

struct k_game_config;

int k_init_game(const struct k_game_config *config);

void k_deinit_game(void);

#endif
