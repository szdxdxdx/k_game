#ifndef K__GAME_INIT_H
#define K__GAME_INIT_H

struct k_game_config;

int k__init_game(const struct k_game_config *config);

void k__deinit_game(const struct k_game_config *config);

#endif
