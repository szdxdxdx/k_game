#include <math.h>

#include "k_game.h"
#include "../core/k_SDL/_public.h"

void k_room_clean_canvas(void *unused) {
    (void)unused;
    SDL_SetRenderDrawColor(k__window.renderer, 30, 30, 30, 255);
    SDL_RenderClear(k__window.renderer);
}

void k_room_draw_grid(void *unused) {
    (void)unused;

    int flashlight_R = 96;
    int grid_size    = 40;

    /* 绘制全屏的网格 */
    {
        SDL_SetRenderDrawColor(k__window.renderer, 40, 40, 40, 255);

        int window_w, window_h;
        SDL_GetWindowSize(k__window.window, &window_w, &window_h);

        int x = grid_size - 1;
        for (; x < window_w; x += grid_size) {
            SDL_RenderDrawLine(k__window.renderer, x, 0, x, window_h);
        }

        int y = grid_size - 1;
        for (; y < window_h; y += grid_size) {
            SDL_RenderDrawLine(k__window.renderer, 0, y, window_w, y);
        }
    }

    /* 绘制鼠标周围的的网格 */
    {
        SDL_SetRenderDrawColor(k__window.renderer, 83, 83, 83, 255);

        int center_x = k_mouse_x();
        int center_y = k_mouse_y();
        int radius_pow_2 = flashlight_R * flashlight_R;

        int left   = (center_x - flashlight_R)     / grid_size * grid_size - 1;
        int right  = (center_x + flashlight_R + 1) / grid_size * grid_size - 1;
        int top    = (center_y - flashlight_R)     / grid_size * grid_size - 1;
        int bottom = (center_y + flashlight_R + 1) / grid_size * grid_size - 1;

        for (; left <= right; left += grid_size) {
            int x = left - center_x;
            int y1 = (int)sqrtf((float)(radius_pow_2 - x * x));
            int y2 = -y1;

            SDL_RenderDrawLine(k__window.renderer, center_x + x, center_y + y1, center_x + x, center_y + y2);
        }

        for (; top <= bottom; top += grid_size) {
            int y = top - center_y;
            int x1 = (int)sqrtf((float)(radius_pow_2 - y * y));
            int x2 = -x1;

            SDL_RenderDrawLine(k__window.renderer, center_x + x1, center_y + y, center_x + x2, center_y + y);
        }
    }
}
