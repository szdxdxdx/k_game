#define K_LOG_TAG "k_game:font"
#include "k_log.h"

#include "k_game/core/k_mem_alloc.h"
#include "k_game/core/k_font.h"

#include "./k_font.h"
#include "./k_font_registry.h"

#include "k_font_builtin_ttf.h"

struct k_font *k__font_builtin;

int k__font_load_builtin(void) {

    struct k_font *font = k__mem_alloc(sizeof(struct k_font));
    if (NULL == font)
        return -1;

    SDL_RWops *rw = SDL_RWFromConstMem(k__font_builtin_ttf, sizeof(k__font_builtin_ttf));
    if (NULL == rw) {
        k__mem_free(font);
        k_log_error("SDL error: %s", SDL_GetError());
        return -1;
    }

    TTF_Font *sdl_font = TTF_OpenFontRW(rw, 1, 18);
    if (NULL == sdl_font) {
        k__mem_free(font);
        k_log_error("SDL error: %s", SDL_GetError());
        return -1;
    }

    font->font = sdl_font;

    k__font_registry_add(font);

    k__font_builtin = font;
    return 0;
}

struct k_font *k_font_get_builtin(void) {
    return k__font_builtin;
}
