#include "k_game.h"

#include "../core/game/_shared.h"
#include "../core/view/_shared.h"

void k_clean_room_canvas(void *unused) {
    (void)unused;
    SDL_SetRenderDrawColor(k__game.renderer, 80, 80, 80, 255);
    SDL_RenderClear(k__game.renderer);
}

void k_draw_grid(void *unused) {
    (void)unused;

    int grid_size = 40;

    SDL_SetRenderDrawColor(k__game.renderer, 100, 100, 100, 255);

    int canvas_w = k__game.canvas_w;
    int canvas_h = k__game.canvas_h;

    int x = grid_size - 1 - (int)k__view.view_x;
    for (; x < canvas_w; x += grid_size) {
        SDL_RenderDrawLine(k__game.renderer, x, 0, x, canvas_h);
    }

    int y = grid_size - 1 - (int)k__view.view_y;
    for (; y < canvas_h; y += grid_size) {
        SDL_RenderDrawLine(k__game.renderer, 0, y, canvas_w, y);
    }
}
