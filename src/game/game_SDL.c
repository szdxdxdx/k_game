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

    const char *title = config->window_title;
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

    if (0 != init_SDL())
        goto init_SDL_failed;

    if (0 != init_SDL_img())
        goto init_SDL_img_failed;

    if (0 != init_SDL_mix())
        goto init_SDL_mix_failed;

    if (0 != init_SDL_ttf())
        goto init_SDL_ttf_failed;

    if (0 != create_window(config))
        goto create_window_failed;

    if (0 != create_renderer())
        goto create_renderer_failed;

    return 0;

deinit:
    destroy_renderer();

create_renderer_failed:
    destroy_window();

create_window_failed:
    deinit_SDL_ttf();

init_SDL_ttf_failed:
    deinit_SDL_mix();

init_SDL_mix_failed:
    deinit_SDL_img();

init_SDL_img_failed:
    deinit_SDL();

init_SDL_failed:

    if (is_init) {
        k_log_error("Failed to initialize SDL");
        return -1;
    }
    else {
        k_log_trace("SDL closed");
        return 0;
    }
}

int k__init_SDL(const struct k_game_config *config) {
    return 0 == init_or_deinit_SDL(config, 1) ? 0 : -1;
}

void k__deinit_SDL(void) {
    init_or_deinit_SDL(NULL, 0);
}

/* endregion */
