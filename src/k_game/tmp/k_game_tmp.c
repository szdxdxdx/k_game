#include "k_game.h"

#include "../core/k_SDL/k_window.h"

void k_clean_room_canvas(void *unused) {
    (void)unused;
    SDL_SetRenderDrawColor(k__window.renderer, 80, 80, 80, 255);
    SDL_RenderClear(k__window.renderer);
}

void k_draw_grid(void *unused) {
    (void)unused;

    int grid_size = 40;

    SDL_SetRenderDrawColor(k__window.renderer, 100, 100, 100, 255);

    int canvas_w = (int)k__window.canvas_w;
    int canvas_h = (int)k__window.canvas_h;

    int x = grid_size - 1 - (int)k__window.view_x;
    for (; x < canvas_w; x += grid_size) {
        SDL_RenderDrawLine(k__window.renderer, x, 0, x, canvas_h);
    }

    int y = grid_size - 1 - (int)k__window.view_y;
    for (; y < canvas_h; y += grid_size) {
        SDL_RenderDrawLine(k__window.renderer, 0, y, canvas_w, y);
    }
}
