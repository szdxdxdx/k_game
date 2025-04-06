#ifndef K__GAME_INTERNAL_H
#define K__GAME_INTERNAL_H

#include "k_game.h"

#include "./k_game_context.h"

int k__init_game(const struct k_game_config *config);

void k__deinit_game(const struct k_game_config *config);

#endif
