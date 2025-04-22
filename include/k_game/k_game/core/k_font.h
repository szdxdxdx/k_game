#ifndef K_FONT_H
#define K_FONT_H

#include "./k_game_fwd.h"

struct k_font *k_font_load(const char *file_path, int font_size);

void k_font_release(struct k_font *font);

#endif
