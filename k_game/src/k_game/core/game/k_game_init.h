#ifndef K__GAME_INIT_H
#define K__GAME_INIT_H

struct k_game_config;

int k__game_init(const struct k_game_config *config);

void k__game_deinit(const struct k_game_config *config);

#endif
