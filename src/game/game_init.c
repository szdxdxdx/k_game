#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "k/flow_step.h"
#include "k/log.h"

#include "k/game.h"
#include "k/internal/game_init.h"
#include "k/internal/game_window.h"

static int init_SDL(void *unused_1, void *unused_2) {
    (void)unused_1;
    (void)unused_2;

    Uint32 flags = SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_AUDIO;
    if (0 != SDL_Init(flags)) {
        k_log_error("SDL_Init(): %s", SDL_GetError());
        return -1;
    }

    k_log_debug("Initialized SDL");
    return 0;
}

static void deinit_SDL(void *unused_1, void *unused_2) {
    (void)unused_1;
    (void)unused_2;

    SDL_Quit();
    k_log_debug("Closed SDL");
}

static int init_SDL_img(void *unused_1, void *unused_2) {
    (void)unused_1;
    (void)unused_2;

    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    if (flags != IMG_Init(flags)) {
        k_log_error("IMG_Init(): %s", IMG_GetError());
        return -1;
    }

    k_log_debug("Initialized SDL_image");
    return 0;
}

static void deinit_SDL_img(void *unused_1, void *unused_2) {
    (void)unused_1;
    (void)unused_2;

    IMG_Quit();
    k_log_debug("Closed SDL_image");
}

static int init_SDL_mix(void *unused_1, void *unused_2) {
    (void)unused_1;
    (void)unused_2;

    int flags = MIX_INIT_MP3;
    if (flags != Mix_Init(flags)) {
        k_log_error("Mix_Init(): %s", Mix_GetError());
        return -1;
    }

    if (0 != Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048)) {
        k_log_error("Mix_OpenAudio(): %s", Mix_GetError());
        return -1;
    }

    Mix_AllocateChannels(32);

    k_log_debug("Initialized SDL_mixer");
    return 0;
}

static void deinit_SDL_mix(void *unused_1, void *unused_2) {
    (void)unused_1;
    (void)unused_2;

    Mix_CloseAudio();
    Mix_Quit();
    k_log_debug("Closed SDL_mixer");
}

static int init_SDL_ttf(void *unused_1, void *unused_2) {
    (void)unused_1;
    (void)unused_2;

    if (0 != TTF_Init()) {
        k_log_error("TTF_Init(): %s", TTF_GetError());
        return -1;
    }

    k_log_debug("Initialized SDL_ttf");
    return 0;
}

static void deinit_SDL_ttf(void *unused_1, void *unused_2) {
    (void)unused_1;
    (void)unused_2;

    TTF_Quit();
    k_log_debug("Closed SDL_ttf");
}

static int create_window(void *unused_1, void *unused_2) {
    (void)unused_1;
    (void)unused_2;

    /* TODO */
    const char *title = "k/game";
    int x = SDL_WINDOWPOS_CENTERED;
    int y = SDL_WINDOWPOS_CENTERED;
    int w = 640;
    int h = 480;
    Uint32 flags = SDL_WINDOW_SHOWN;

    if (NULL == (k_window->window = SDL_CreateWindow(title, x, y, w, h, flags))) {
        k_log_error("SDL_CreateWindow(): %s", SDL_GetError());
        return -1;
    }

    k_log_debug("Created game window");
    return 0;
}

static void destroy_window(void *unused_1, void *unused_2) {
    (void)unused_1;
    (void)unused_2;

    if (NULL != k_window->renderer) {
        SDL_DestroyRenderer(k_window->renderer);
        k_log_debug("Destroyed game window renderer");
    }

    if (NULL != k_window->window) {
        SDL_DestroyWindow(k_window->window);
        k_log_debug("Destroyed game window");
    }
}

static int create_renderer(void *unused_1, void *unused_2) {
    (void)unused_1;
    (void)unused_2;

    Uint32 flags = SDL_RENDERER_ACCELERATED;

    if (NULL == (k_window->renderer = SDL_CreateRenderer(k_window->window, -1, flags))) {
        k_log_error("SDL_CreateRenderer(): %s", SDL_GetError());
        return -1;
    }

    k_log_debug("Created game window renderer");
    return 0;
}

static void destroy_renderer(void *unused_1, void *unused_2) {
    (void)unused_1;
    (void)unused_2;

    if (NULL != k_window->renderer) {
        SDL_DestroyRenderer(k_window->renderer);
        k_log_debug("Destroyed game window renderer");
    }
}

static int run_setup_callback(void *global_data, void *unused) {
    const struct k_game_config *game_config = global_data;

    int result = game_config->fn_game_setup();
    if (0 != result) {
        k_log_error("fn_game_setup() return %d", result);
        return -1;
    }

    return 0;
}

static const struct k_flow_step steps_to_init[] = {
    { NULL, init_SDL,           deinit_SDL       },
    { NULL, init_SDL_img,       deinit_SDL_img   },
    { NULL, init_SDL_mix,       deinit_SDL_mix   },
    { NULL, init_SDL_ttf,       deinit_SDL_ttf   },
    { NULL, create_window,      destroy_window   },
    { NULL, create_renderer,    destroy_renderer },
    { NULL, run_setup_callback, NULL             }
};

#define array_len(x) (sizeof(x) / sizeof(x[0]))

int k_init_game(const struct k_game_config *config) {

    size_t steps_num = array_len(steps_to_init);
    size_t inited_num = k_execute_forward_flow(steps_to_init, steps_num, (void *)config);

    if (inited_num != steps_num) {
        k_log_error("Failed to initialize game");
        goto err;
    }

    return 0;

err:
    k_execute_backward_flow(steps_to_init, inited_num, NULL);
    return -1;
}

void k_deinit_game(void) {
    k_execute_backward_flow(steps_to_init, array_len(steps_to_init), NULL);
}
