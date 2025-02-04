#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "k/log.h"

#include "k/game.h"
#include "_internal_.h"

struct k_SDL_context {
    unsigned int is_inited_SDL       : 1;
    unsigned int is_inited_SDL_img   : 1;
    unsigned int is_inited_SDL_mix   : 1;
    unsigned int is_inited_SDL_ttf   : 1;
    unsigned int is_created_window   : 1;
    unsigned int is_created_renderer : 1;
    unsigned int is_inited_module    : 1;
};

static struct k_SDL_context context = {
    .is_inited_SDL       = 0,
    .is_inited_SDL_img   = 0,
    .is_inited_SDL_mix   = 0,
    .is_inited_SDL_ttf   = 0,
    .is_created_window   = 0,
    .is_created_renderer = 0,
    .is_inited_module    = 0,
};

static int init_SDL(void) {

    Uint32 flags = SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_AUDIO;
    if (0 != SDL_Init(flags)) {
        k_log_error("SDL_Init(): %s", SDL_GetError());
        return -1;
    }

    context.is_inited_SDL = 1;
    return 0;
}

static void deinit_SDL(void) {

    if ( ! context.is_inited_SDL)
        return;

    SDL_Quit();
    context.is_inited_SDL = 0;
}

static int init_SDL_img(void) {

    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    if (flags != IMG_Init(flags)) {
        k_log_error("IMG_Init(): %s", IMG_GetError());
        return -1;
    }

    context.is_inited_SDL_img = 1;
    return 0;
}

static void deinit_SDL_img(void) {

    if ( ! context.is_inited_SDL_img)
        return;

    IMG_Quit();
    context.is_inited_SDL_img = 0;
}

static int init_SDL_mix(void) {

    int flags = MIX_INIT_MP3;
    if (flags != Mix_Init(flags)) {
        k_log_error("Mix_Init(): %s", Mix_GetError());
        return -1;
    }

    if (0 != Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048)) {
        Mix_Quit();
        k_log_error("Mix_OpenAudio(): %s", Mix_GetError());
        return -1;
    }

    Mix_AllocateChannels(32);

    context.is_inited_SDL_mix = 1;
    return 0;
}

static void deinit_SDL_mix(void) {

    if ( ! context.is_inited_SDL_mix)
        return;

    Mix_CloseAudio();
    Mix_Quit();
    context.is_inited_SDL_mix = 0;
}

static int init_SDL_ttf(void) {

    if (0 != TTF_Init()) {
        k_log_error("TTF_Init(): %s", TTF_GetError());
        return -1;
    }

    context.is_inited_SDL_ttf = 1;
    return 0;
}

static void deinit_SDL_ttf(void) {

    if ( ! context.is_inited_SDL_ttf)
        return;

    TTF_Quit();
    context.is_inited_SDL_ttf = 0;
}

static int create_window(const struct k_game_config *config) {

    const char *title = "k/game"; /* <- tmp */
    int x = SDL_WINDOWPOS_CENTERED;
    int y = SDL_WINDOWPOS_CENTERED;
    int w = config->window_w;
    int h = config->window_h;
    Uint32 flags = SDL_WINDOW_SHOWN;

    if (NULL == (k__game->window = SDL_CreateWindow(title, x, y, w, h, flags))) {
        k_log_error("SDL_CreateWindow(): %s", SDL_GetError());
        return -1;
    }

    context.is_created_window = 1;
    return 0;
}

static void destroy_window(void) {

    if ( ! context.is_created_window)
        return;

    SDL_DestroyWindow(k__game->window);
    k__game->window = NULL;
    context.is_created_window = 0;
}

static int create_renderer(void) {

    Uint32 flags = SDL_RENDERER_ACCELERATED;

    if (NULL == (k__game->renderer = SDL_CreateRenderer(k__game->window, -1, flags))) {
        k_log_error("SDL_CreateRenderer(): %s", SDL_GetError());
        return -1;
    }

    context.is_created_renderer = 1;
    return 0;
}

static void destroy_renderer(void) {

    if ( ! context.is_created_renderer)
        return;

    SDL_DestroyRenderer(k__game->renderer);
    k__game->renderer = NULL;

    context.is_created_renderer = 0;
}

static void deinit_module(void) {

    destroy_renderer();
    destroy_window();
    deinit_SDL_ttf();
    deinit_SDL_mix();
    deinit_SDL_img();
    deinit_SDL();

    context.is_inited_module = 0;
}

int k__init_SDL(const struct k_game_config *config) {

    if (context.is_inited_module) {
        k_log_error("SDL is already initialized");
        return -1;
    }

    int r = init_SDL()
         || init_SDL_img()
         || init_SDL_mix()
         || init_SDL_ttf()
         || create_window(config)
         || create_renderer();

    if (0 != r) {
        k_log_error("Failed to initialize SDL");

        deinit_module();
        return -1;
    }

    context.is_inited_module = 1;

    k_log_trace("SDL initialized");
    return 0;
}

void k__deinit_SDL(void) {

    if ( ! context.is_inited_module)
        return;

    deinit_module();
    k_log_trace("SDL closed");
}
