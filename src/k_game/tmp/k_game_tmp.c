#include "k_game.h"
#include "../core/room/_shared.h"
#include "../core/k_SDL/_shared.h"
#include "../core/view/_shared.h"

void k_clean_room_canvas(void *unused) {
    (void)unused;
    SDL_SetRenderDrawColor(k__SDL.renderer, 80, 80, 80, 255);
    SDL_RenderClear(k__SDL.renderer);
}

void k_draw_grid(void *unused) {
    (void)unused;

    int grid_size = 40;

    SDL_SetRenderDrawColor(k__SDL.renderer, 100, 100, 100, 255);

    int canvas_w = k__SDL.canvas_w;
    int canvas_h = k__SDL.canvas_h;

    int x = grid_size - 1 - (int)k__view.view_x;
    for (; x < canvas_w; x += grid_size) {
        SDL_RenderDrawLine(k__SDL.renderer, x, 0, x, canvas_h);
    }

    int y = grid_size - 1 - (int)k__view.view_y;
    for (; y < canvas_h; y += grid_size) {
        SDL_RenderDrawLine(k__SDL.renderer, 0, y, canvas_w, y);
    }
}
