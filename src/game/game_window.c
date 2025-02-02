#include "SDL.h"

#include "k/log.h"

#include "k/internal/game_window.h"

struct k_game_window * const k_window = &(struct k_game_window) {
    .window = NULL,
    .renderer = NULL,
};

static int create_window(void) {

    /* TODO */
    const char *title = "k/game";
    int x = SDL_WINDOWPOS_CENTERED;
    int y = SDL_WINDOWPOS_CENTERED;
    int w = 640;
    int h = 480;
    Uint32 flags = SDL_WINDOW_SHOWN;

    SDL_Window *window = SDL_CreateWindow(title, x, y, w, h, flags);
    if (NULL == window) {
        k_log_error("SDL_CreateWindow(): %s", SDL_GetError());
        return -1;
    }

    k_window->window = window;

    k_log_debug("Created game window");
    return 0;
}

static void destroy_window(void) {
    SDL_DestroyWindow(k_window->window);
    k_log_debug("Destroyed game window");
}

static int create_renderer(void) {

    Uint32 flags = SDL_RENDERER_ACCELERATED;

    SDL_Renderer *renderer = SDL_CreateRenderer(k_window->window, -1, flags);
    if (NULL == renderer) {
        k_log_error("SDL_CreateRenderer(): %s", SDL_GetError());
        return -1;
    }

    k_window->renderer = renderer;

    k_log_debug("Created game window renderer");
    return 0;
}

static void destroy_renderer(void) {
    SDL_DestroyRenderer(k_window->renderer);
    k_log_debug("Destroyed game window renderer");
}

int k_create_window(void) {

    /* TODO 初始化失败时返回非 0 */

    create_window();
    create_renderer();

    return 0;
}

void k_destroy_window(void) {

    /* TODO 增强容错，调用反初始化函数始终是安全的 */

    destroy_renderer();
    destroy_window();
}
