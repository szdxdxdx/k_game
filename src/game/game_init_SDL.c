#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "k/log.h"

#include "k/internal/game_init_SDL.h"

static int init_SDL(void) {

    Uint32 flags = SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_AUDIO;
    if (0 != SDL_Init(flags)) {
        k_log_error("SDL_Init(): %s", SDL_GetError());
        return -1;
    }

    k_log_debug("Initialized SDL");
    return 0;
}

static void deinit_SDL(void) {
    SDL_Quit();
    k_log_debug("Closed SDL");
}

static int init_SDL_img(void) {

    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    if (flags == IMG_Init(flags)) {
        k_log_error("IMG_Init(): %s", IMG_GetError());
        return -1;
    }

    k_log_debug("Initialized SDL_image");
    return 0;
}

static void deinit_SDL_img(void) {
    IMG_Quit();
    k_log_debug("Closed SDL_image");
}

static int init_SDL_mix(void) {

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

static void deinit_SDL_mix(void) {
    Mix_CloseAudio();
    Mix_Quit();
    k_log_debug("Closed SDL_mixer");
}

static int init_SDL_ttf(void) {

    if (0 != TTF_Init()) {
        k_log_error("TTF_Init(): %s", TTF_GetError());
        return -1;
    }

    k_log_debug("Initialized SDL_ttf");
    return 0;
}

static void deinit_SDL_ttf(void) {
    TTF_Quit();
    k_log_debug("Closed SDL_ttf");
}

int k_init_SDL(void) {

    /* TODO 初始化失败时返回非 0 */

    init_SDL();
    init_SDL_img();
    init_SDL_mix();
    init_SDL_ttf();
    return 0;
}

void k_deinit_SDL(void) {

    /* TODO 增强容错，调用反初始化函数始终是安全的 */

    deinit_SDL_ttf();
    deinit_SDL_mix();
    deinit_SDL_img();
    deinit_SDL();
}
