#ifndef K_GAME_INTERNAL_H
#define K_GAME_INTERNAL_H

#include "k_game.h"

#include "./_shared.h"

int k__init_game(const struct k_game_config *config);

void k__deinit_game(const struct k_game_config *config);

#endif
