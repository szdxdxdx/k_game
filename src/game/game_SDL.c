#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "k/log.h"

#include "k/game.h"
#include "_internal_.h"

static int init_SDL(void) {

    Uint32 flags = SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_AUDIO;
    if (0 != SDL_Init(flags)) {
        k_log_error("SDL_Init(): %s", SDL_GetError());
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
        k_log_error("IMG_Init(): %s", IMG_GetError());
        return -1;
    }

    return 0;
}

static void deinit_SDL_img(void) {
    IMG_Quit();
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

    return 0;
}

static void deinit_SDL_mix(void) {
    Mix_CloseAudio();
    Mix_Quit();
}

static int init_SDL_ttf(void) {

    if (0 != TTF_Init()) {
        k_log_error("TTF_Init(): %s", TTF_GetError());
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
        k_log_error("SDL_CreateWindow(): %s", SDL_GetError());
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
        k_log_error("SDL_CreateRenderer(): %s", SDL_GetError());
        return -1;
    }

    return 0;
}

static void destroy_renderer(void) {
    SDL_DestroyRenderer(k__game->renderer);
    k__game->renderer = NULL;
}

struct k__SDL_init_flags {
    unsigned int is_inited_SDL       : 1;
    unsigned int is_inited_SDL_img   : 1;
    unsigned int is_inited_SDL_mix   : 1;
    unsigned int is_inited_SDL_ttf   : 1;
    unsigned int is_created_window   : 1;
    unsigned int is_created_renderer : 1;
    unsigned int is_inited_module    : 1;
};

static struct k__SDL_init_flags init_flags = {
    .is_inited_SDL       = 0,
    .is_inited_SDL_img   = 0,
    .is_inited_SDL_mix   = 0,
    .is_inited_SDL_ttf   = 0,
    .is_created_window   = 0,
    .is_created_renderer = 0,
    .is_inited_module    = 0,
};

static void deinit_module(void) {

    if (init_flags.is_created_renderer)
        destroy_renderer();
    if (init_flags.is_created_window)
        destroy_window();
    if (init_flags.is_inited_SDL_ttf)
        deinit_SDL_ttf();
    if (init_flags.is_inited_SDL_mix)
        deinit_SDL_mix();
    if (init_flags.is_inited_SDL_img)
        deinit_SDL_img();
    if (init_flags.is_inited_SDL)
        deinit_SDL();

    init_flags.is_inited_module = 0;
}

int k__init_SDL(const struct k_game_config *config) {

    if (init_flags.is_inited_module)
        goto done;

    if (0 != init_SDL()) {
        k_log_error("Failed to initialize SDL");
        goto err;
    }
    init_flags.is_inited_SDL = 1;

    if (0 != init_SDL_img()) {
        k_log_error("Failed to initialize SDL_img");
        goto err;
    }
    init_flags.is_inited_SDL_img = 1;

    if (0 != init_SDL_mix()) {
        k_log_error("Failed to initialize SDL_mix");
        goto err;
    }
    init_flags.is_inited_SDL_mix = 1;

    if (0 != init_SDL_ttf()) {
        k_log_error("Failed to initialize SDL_ttf");
        goto err;
    }
    init_flags.is_inited_SDL_ttf = 1;

    if (0 != create_window(config)) {
        k_log_error("Failed to create window");
        goto err;
    }
    init_flags.is_created_window = 1;

    if (0 != create_renderer()) {
        k_log_error("Failed to create renderer");
        goto err;
    }
    init_flags.is_created_renderer = 1;

    init_flags.is_inited_module = 1;

done:
    k_log_trace("SDL initialized");
    return 0;

err:
    k_log_error("Failed to initialize SDL");

    deinit_module();
    return -1;
}

void k__deinit_SDL(void) {

    if (init_flags.is_inited_module)
        deinit_module();

    k_log_trace("SDL closed");
}
