#ifndef K__FONT_H
#define K__FONT_H

#include "SDL_ttf.h"

#include "../asset/k_asset_registry.h"

struct k_font {

    struct k_asset_registry_node registry_node;

    TTF_Font *font;
};

#endif
