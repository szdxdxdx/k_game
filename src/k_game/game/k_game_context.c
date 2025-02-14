#include "./k_game_context.h"

struct k_game_context k_game = {
    .window     = NULL,
    .renderer   = NULL,
    .quit_game  = 0,
    .delta_time = 0.0f
};
