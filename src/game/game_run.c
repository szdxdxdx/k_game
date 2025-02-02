#include "k/game.h"
#include "k/internal/game_init.h"
#include "k/internal/game_window.h"
#include "k/internal/room_registry.h"


int k_run_game(const struct k_game_config *config) {

    if (0 != k_init_game(config))
        return -1;

    k_deinit_game();
    return 0;
}
