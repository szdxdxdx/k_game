#include "k/game.h"

#include "k/internal/SDL_env.h"

int k_run_game(const struct k_game_config *config) {

    if (0 != k_init_SDL())
        return -1;


    k_deinit_SDL();
    return 0;
}
