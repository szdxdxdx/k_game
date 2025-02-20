#ifndef K_GAME__SDL_INIT_H
#define K_GAME__SDL_INIT_H

struct k_game_config;

int k__init_SDL(const struct k_game_config *config);

void k__close_SDL(void);

#endif
