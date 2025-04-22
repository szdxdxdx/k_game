#include "SDL_ttf.h"

#define K_LOG_TAG "k_game:font"
#include "k_log.h"

#include "k_game/core/k_mem_alloc.h"
#include "k_game/core/k_font.h"

#include "./k_font.h"
#include "./k_font_registry.h"

struct k_font *k_font_load(const char *file_path, int font_size) {

    struct k_font *font = k__mem_alloc(sizeof(struct k_font));
    if (NULL == font)
        goto err;

    TTF_Font *sdl_font = TTF_OpenFont(file_path, font_size);
    if (NULL == sdl_font) {
        k__mem_free(font);
        k_log_error("SDL error: %s\n", TTF_GetError());
        goto err;
    }

    k__font_registry_add(font);
    return font;

err:
    k_log_error("Failed to load font");
    return NULL;
}

void k_font_release(struct k_font *font) {

    if (NULL == font)
        return;

    TTF_CloseFont(font->font);
    k__font_registry_del(font);
    k__mem_free(font);
}
