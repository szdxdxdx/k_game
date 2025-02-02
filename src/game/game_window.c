#include "SDL.h"

#include "k/internal/game_window.h"

struct k_game_window * const k_window = &(struct k_game_window) {
    .window   = NULL,
    .renderer = NULL,
};
