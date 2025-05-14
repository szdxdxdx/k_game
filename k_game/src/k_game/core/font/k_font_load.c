#include "SDL_ttf.h"

#define K_LOG_TAG "k_game:font"
#include "k_log.h"

#include "k_game/core/k_mem_alloc.h"
#include "k_game/core/k_font.h"

#include "./k_font.h"
#include "./k_font_registry.h"

struct k_font *k_font_load(const char *file_path, int point_size) {

    if (NULL == file_path || '\0' == file_path[0])
        return NULL;
    if (point_size <= 0)
        return NULL;

    struct k_font *font = k__mem_alloc(sizeof(struct k_font));
    if (NULL == font)
        return NULL;

    TTF_Font *sdl_font = TTF_OpenFont(file_path, point_size);
    if (NULL == sdl_font) {
        k__mem_free(font);
        k_log_error("SDL error: %s\n", TTF_GetError());
        return NULL;
    }

    font->font = sdl_font;

    k__font_registry_add(font);
    return font;
}

void k_font_release(struct k_font *font) {

    if (NULL == font)
        return;

    TTF_CloseFont(font->font);
    k__font_registry_del(font);
    k__mem_free(font);
}
