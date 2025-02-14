#ifndef K_GAME__GAME_SDL_H
#define K_GAME__GAME_SDL_H

#include "k_game/game.h"

int k_game_SDL_init(const struct k_game_config *config);

void k_game_SDL_deinit(void);

#endif
