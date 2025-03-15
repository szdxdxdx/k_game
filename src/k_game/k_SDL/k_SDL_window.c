#include "k_game_room.h"

#include "./k_SDL.h"

struct k_game_window k__window = {
    .window   = NULL,
    .renderer = NULL,
};

void k_room_clean_canvas(void *unused) {
    (void)unused;
    SDL_SetRenderDrawColor(k__window.renderer, 30, 30, 30, 255);
    SDL_RenderClear(k__window.renderer);
}
