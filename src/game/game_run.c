#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "k/flow_step.h"
#include "k/log.h"

#include "k/game.h"
#include "_internal_.h"

struct k_game_context * const k_game = &(struct k_game_context) {
    .quit_game = 0,
};

struct k_game_window * const k_window = &(struct k_game_window) {
    .window   = NULL,
    .renderer = NULL,
};

const struct k_game_config K_GAME_CONFIG_INIT = {
    .window_h = 480,
    .window_w = 640,
    .fn_setup_game = NULL
};

static int check_game_config(const struct k_game_config *config) {

    #define GAME_CONFIG_ASSERT(cond) \
        do { \
            if ( ! (cond)) { \
                k_log_error("Invalid game config, assert( " #cond " )"); \
                return -1; \
            } \
        } while(0)

    GAME_CONFIG_ASSERT(NULL != config);
    GAME_CONFIG_ASSERT(0 < config->window_h);
    GAME_CONFIG_ASSERT(0 < config->window_w);
    GAME_CONFIG_ASSERT(NULL != config->fn_setup_game);

    #undef GAME_CONFIG_ASSERT
    return 0;
}

static int init_SDL(void *game_config, void *unused) {
    (void)game_config;
    (void)unused;

    Uint32 flags = SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_AUDIO;
    if (0 != SDL_Init(flags)) {
        k_log_error("SDL_Init(): %s", SDL_GetError());
        return -1;
    }

    k_log_debug("Initialized SDL");
    return 0;
}

static void deinit_SDL(void *game_config, void *unused) {
    (void)game_config;
    (void)unused;

    SDL_Quit();
    k_log_debug("Closed SDL");
}

static int init_SDL_img(void *game_config, void *unused) {
    (void)game_config;
    (void)unused;

    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    if (flags != IMG_Init(flags)) {
        k_log_error("IMG_Init(): %s", IMG_GetError());
        return -1;
    }

    k_log_debug("Initialized SDL_image");
    return 0;
}

static void deinit_SDL_img(void *game_config, void *unused) {
    (void)game_config;
    (void)unused;

    IMG_Quit();
    k_log_debug("Closed SDL_image");
}

static int init_SDL_mix(void *game_config, void *unused) {
    (void)game_config;
    (void)unused;

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

static void deinit_SDL_mix(void *game_config, void *unused) {
    (void)game_config;
    (void)unused;

    Mix_CloseAudio();
    Mix_Quit();
    k_log_debug("Closed SDL_mixer");
}

static int init_SDL_ttf(void *game_config, void *unused) {
    (void)game_config;
    (void)unused;

    if (0 != TTF_Init()) {
        k_log_error("TTF_Init(): %s", TTF_GetError());
        return -1;
    }

    k_log_debug("Initialized SDL_ttf");
    return 0;
}

static void deinit_SDL_ttf(void *game_config, void *unused) {
    (void)game_config;
    (void)unused;

    TTF_Quit();
    k_log_debug("Closed SDL_ttf");
}

static int create_window(void *game_config, void *unused) {
    (void)unused;

    const struct k_game_config *config = game_config;

    /* TODO */
    const char *title = "k/game";
    int x = SDL_WINDOWPOS_CENTERED;
    int y = SDL_WINDOWPOS_CENTERED;
    int w = config->window_w;
    int h = config->window_h;
    Uint32 flags = SDL_WINDOW_SHOWN;

    if (NULL == (k_window->window = SDL_CreateWindow(title, x, y, w, h, flags))) {
        k_log_error("SDL_CreateWindow(): %s", SDL_GetError());
        return -1;
    }

    k_log_debug("Created game window");
    return 0;
}

static void destroy_window(void *game_config, void *unused) {
    (void)game_config;
    (void)unused;

    if (NULL != k_window->window) {
        SDL_DestroyWindow(k_window->window);
        k_window->window = NULL;
        k_log_debug("Destroyed game window");
    }
}

static int create_renderer(void *game_config, void *unused) {
    (void)game_config;
    (void)unused;

    Uint32 flags = SDL_RENDERER_ACCELERATED;

    if (NULL == (k_window->renderer = SDL_CreateRenderer(k_window->window, -1, flags))) {
        k_log_error("SDL_CreateRenderer(): %s", SDL_GetError());
        return -1;
    }

    k_log_debug("Created game window renderer");
    return 0;
}

static void destroy_renderer(void *game_config, void *unused) {
    (void)game_config;
    (void)unused;

    if (NULL != k_window->renderer) {
        SDL_DestroyRenderer(k_window->renderer);
        k_window->renderer = NULL;
        k_log_debug("Destroyed game window renderer");
    }
}

static int init_room_registry(void *game_config, void *unused) {
    (void)game_config;
    (void)unused;

    if (0 != k_init_room_registry()) {
        k_log_error("Failed to initialize game room registry");
        return -1;
    }

    return 0;
}

static void deinit_room_registry(void *game_config, void *unused) {
    (void)game_config;
    (void)unused;
    k_deinit_room_registry();
}

static int run_setup_callback(void *game_config, void *unused) {
    (void)unused;

    const struct k_game_config *config = game_config;

    int result = config->fn_setup_game();
    if (0 != result) {
        k_log_error("fn_setup_game() return %d", result);
        return -1;
    }

    return 0;
}

static const struct k_flow_step steps_to_init[] = {
    { NULL, init_SDL,           deinit_SDL           },
    { NULL, init_SDL_img,       deinit_SDL_img       },
    { NULL, init_SDL_mix,       deinit_SDL_mix       },
    { NULL, init_SDL_ttf,       deinit_SDL_ttf       },
    { NULL, create_window,      destroy_window       },
    { NULL, create_renderer,    destroy_renderer     },
    { NULL, init_room_registry, deinit_room_registry },
    { NULL, run_setup_callback, NULL                 },
};

#define array_len(x) (sizeof(x) / sizeof(x[0]))

static int init_game(const struct k_game_config *config) {

    if (0 != check_game_config(config))
        return -1;

    size_t steps_num = array_len(steps_to_init);
    size_t inited_num = k_execute_flow_forward(steps_to_init, steps_num, (void *)config);

    if (inited_num != steps_num) {
        k_log_error("Failed to initialize game");
        goto err;
    }

    return 0;

err:
    k_execute_flow_backward(steps_to_init, inited_num, NULL);
    return -1;
}

static void deinit_game(void) {
    k_execute_flow_backward(steps_to_init, array_len(steps_to_init), NULL);
}

static void run_room(void) {

    struct k_room *room = k_room_stack_get_top();
    if (NULL == room) {
        k_log_error("No room to run");
        return;
    }

    k_run_room(room);
}

int k_run_game(const struct k_game_config *config) {

    if (0 != init_game(config))
        return -1;

    run_room();

    deinit_game();
    return 0;
}
