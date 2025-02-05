#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "k/log.h"

#include "k/game.h"
#include "_internal_.h"

/* region [SDL] */

static int init_SDL(void) {

    Uint32 flags = SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_AUDIO;
    if (0 != SDL_Init(flags)) {
        k_log_error("Failed to initialize SDL: %s", SDL_GetError());
        return -1;
    }

    return 0;
}

static void deinit_SDL(void) {
    SDL_Quit();
}

static int init_SDL_img(void) {

    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    if (flags != IMG_Init(flags)) {
        k_log_error("Failed to initialize SDL_img: %s", IMG_GetError());
        return -1;
    }

    return 0;
}

static void deinit_SDL_img(void) {
    IMG_Quit();
}

static int init_SDL_mix(void) {

    int flags = MIX_INIT_MP3;
    if (flags != Mix_Init(flags))
        goto err;

    if (0 != Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048)) {
        Mix_Quit();
        goto err;
    }

    Mix_AllocateChannels(32);

    return 0;

err:
    k_log_error("Failed to initialize SDL_mix: %s", Mix_GetError());
    return -1;
}

static void deinit_SDL_mix(void) {
    Mix_CloseAudio();
    Mix_Quit();
}

static int init_SDL_ttf(void) {

    if (0 != TTF_Init()) {
        k_log_error("Failed to initialize SDL_ttf: %s", TTF_GetError());
        return -1;
    }

    return 0;
}

static void deinit_SDL_ttf(void) {
    TTF_Quit();
}

static int create_window(const struct k_game_config *config) {

    const char *title = "k/game"; /* <- tmp */
    int x = SDL_WINDOWPOS_CENTERED;
    int y = SDL_WINDOWPOS_CENTERED;
    int w = config->window_w;
    int h = config->window_h;
    Uint32 flags = SDL_WINDOW_SHOWN;

    if (NULL == (k__game->window = SDL_CreateWindow(title, x, y, w, h, flags))) {
        k_log_error("Failed to create game window: %s", SDL_GetError());
        return -1;
    }

    return 0;
}

static void destroy_window(void) {
    SDL_DestroyWindow(k__game->window);
    k__game->window = NULL;
}

static int create_renderer(void) {

    Uint32 flags = SDL_RENDERER_ACCELERATED;

    if (NULL == (k__game->renderer = SDL_CreateRenderer(k__game->window, -1, flags))) {
        k_log_error("Failed to create game window renderer: %s", SDL_GetError());
        return -1;
    }

    return 0;
}

static void destroy_renderer(void) {
    SDL_DestroyRenderer(k__game->renderer);
    k__game->renderer = NULL;
}

/* endregion */

/* region [init] */

static int init_or_deinit_SDL(const struct k_game_config *config, int is_init) {

    if ( ! is_init)
        goto deinit;

    if (0 != init_SDL())            goto err_init_SDL;
    if (0 != init_SDL_img())        goto err_init_SDL_img;
    if (0 != init_SDL_mix())        goto err_init_SDL_mix;
    if (0 != init_SDL_ttf())        goto err_init_SDL_ttf;
    if (0 != create_window(config)) goto err_create_window;
    if (0 != create_renderer())     goto err_create_renderer;

    return 0;

deinit:              destroy_renderer();
err_create_renderer: destroy_window();
err_create_window:   deinit_SDL_ttf();
err_init_SDL_ttf:    deinit_SDL_mix();
err_init_SDL_mix:    deinit_SDL_img();
err_init_SDL_img:    deinit_SDL();
err_init_SDL:

    return is_init ? -1 : 0;
}

static int is_initialized = 0;

int k__init_SDL(const struct k_game_config *config) {

    if (is_initialized) {
        k_log_error("SDL already initialized");
        return -1;
    }

    if (0 != init_or_deinit_SDL(config, 1)) {
        k_log_error("Failed to initialize SDL");
        return -1;
    }

    is_initialized = 1;

    k_log_trace("SDL initialized");
    return 0;
}

void k__deinit_SDL(void) {

    if ( ! is_initialized)
        return;

    init_or_deinit_SDL(NULL, 0);
    is_initialized = 0;

    k_log_trace("SDL closed");
}

/* endregion */
