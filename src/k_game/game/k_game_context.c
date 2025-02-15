#include "./k_game_context.h"

struct k_game_context k_game = {
    .window     = NULL,
    .renderer   = NULL,
    .quit_game  = 0,
    .delta_time = 0.0f
};

struct k_room *k_get_current_room(void) {
    return k_game.current_room;
}
