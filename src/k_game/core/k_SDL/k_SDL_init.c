#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "k_log.h"
#include "k_seq_step.h"

#include "./_internal.h"

/* region [steps] */

static int step_before_init(void *unused) {
    (void)unused;

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
    return 0;
}

static int step_init_SDL(void *unused) {
    (void)unused;

    Uint32 flags = SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_AUDIO;
    if (0 != SDL_Init(flags)) {
        k_log_error("Failed to initialize SDL: %s", SDL_GetError());
        return -1;
    }

    return 0;
}

static void step_quit_SDL(void *unused) {
    (void)unused;

    SDL_Quit();
}

static int step_init_SDL_img(void *unused) {
    (void)unused;

    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    if (flags != IMG_Init(flags)) {
        k_log_error("Failed to initialize SDL_img: %s", IMG_GetError());
        return -1;
    }

    return 0;
}

static void step_quit_SDL_img(void *unused) {
    (void)unused;

    IMG_Quit();
}

static int step_init_SDL_mix(void *unused) {
    (void)unused;

    int flags = MIX_INIT_MP3;
    if (flags != Mix_Init(flags))
        goto err;

    if (0 != Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048)) {
        Mix_Quit();
        goto err;
    }

    Mix_AllocateChannels(48);

    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    Mix_Volume(-1, MIX_MAX_VOLUME / 4);
    return 0;

err:
    k_log_error("Failed to initialize SDL_mix: %s", Mix_GetError());
    return -1;
}

static void step_quit_SDL_mix(void *unused) {
    (void)unused;

    Mix_CloseAudio();
    Mix_Quit();
}

static int step_init_SDL_ttf(void *unused) {
    (void)unused;

    if (0 != TTF_Init()) {
        k_log_error("Failed to initialize SDL_ttf: %s", TTF_GetError());
        return -1;
    }

    return 0;
}

static void step_quit_SDL_ttf(void *unused) {
    (void)unused;

    TTF_Quit();
}

static int step_create_window(void *config_) {
    const struct k_game_config *config = config_;

    if (config->window_w <= 0 || config->window_h <= 0) {
        k_log_error("Failed to create game window: invalid window size");
        return -1;
    }

    const char *title = config->window_title;
    int x = SDL_WINDOWPOS_CENTERED;
    int y = SDL_WINDOWPOS_CENTERED;
    int w = config->window_w;
    int h = config->window_h;
    Uint32 flags = SDL_WINDOW_SHOWN;

    SDL_Window *window = SDL_CreateWindow(title, x, y, w, h, flags);
    if (NULL == window) {
        k_log_error("Failed to create game window: %s", SDL_GetError());
        return -1;
    }

    k__SDL.window   = window;
    k__SDL.window_w = w;
    k__SDL.window_h = h;
    return 0;
}

static void step_destroy_window(void *unused) {
    (void)unused;

    SDL_DestroyWindow(k__SDL.window);
}

static int step_create_renderer(void *unused) {
    (void)unused;

    Uint32 flags = SDL_RENDERER_ACCELERATED;

    SDL_Renderer *renderer = SDL_CreateRenderer(k__SDL.window, -1, flags);
    if (NULL == renderer) {
        k_log_error("Failed to create game window renderer: %s", SDL_GetError());
        return -1;
    }

    if (0 != SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND)) {
        k_log_error("Failed to set SDL renderer blend mode: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        return -1;
    }

    k__SDL.renderer = renderer;
    return 0;
}

static void step_destroy_renderer(void *unused) {
    (void)unused;

    SDL_DestroyRenderer(k__SDL.renderer);
}

static int step_create_canvas(void *config_) {
    struct k_game_config *config = config_;

    int canvas_w = config->canvas_w;
    int canvas_h = config->canvas_h;

    if (canvas_w == 0 && canvas_h == 0) {
        canvas_w = config->window_w;
        canvas_h = config->window_h;
    }
    else {
        if (canvas_w <= 0 || canvas_h <= 0) {
            k_log_error("Failed to create game canvas: invalid canvas size");
            return -1;
        }
    }

    Uint32 format = SDL_PIXELFORMAT_RGBA8888;
    int    access = SDL_TEXTUREACCESS_TARGET;
    SDL_Texture* canvas = SDL_CreateTexture(k__SDL.renderer, format, access, canvas_w, canvas_h);
    if (NULL == canvas) {
        k_log_error("Failed to create game canvas: %s", SDL_GetError());
        return -1;
    }

    k__SDL.canvas   = canvas;
    k__SDL.canvas_w = canvas_w;
    k__SDL.canvas_h = canvas_h;
    return 0;
}

static void step_destroy_canvas(void *unused) {
    (void)unused;

    SDL_DestroyTexture(k__SDL.canvas);
}

static struct k_seq_step steps[] = {
    { step_before_init,     NULL                  },
    { step_init_SDL,        step_quit_SDL         },
    { step_init_SDL_img,    step_quit_SDL_img     },
    { step_init_SDL_mix,    step_quit_SDL_mix     },
    { step_init_SDL_ttf,    step_quit_SDL_ttf     },
    { step_create_window,   step_destroy_window   },
    { step_create_renderer, step_destroy_renderer },
    { step_create_canvas,   step_destroy_canvas   },
};

/* endregion */

int k__init_SDL(const struct k_game_config *config) {

    if (0 != k_seq_step_exec(steps, k_seq_step_array_len(steps), (void *)config)) {
        k_log_error("Failed to initialize SDL");
        return -1;
    }

    return 0;
}

void k__quit_SDL(void) {
    k_seq_step_exec_backward(steps, k_seq_step_array_len(steps), NULL);
}
